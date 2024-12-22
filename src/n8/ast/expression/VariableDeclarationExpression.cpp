/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/expression/VariableDeclarationExpression.hpp>
#include <n8/core/Runtime.hpp>
#include <n8/core/SymbolTable.hpp>
#include <n8/util/PathHelper.hpp>

#include <bit>
#include <filesystem>

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#   include <dlfcn.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <Windows.h>
#else
#   error "Unsupported architecture for shared objects or dynamic libraries."
#endif

DynamicObject VariableDeclarationExpression::visit(SymbolTable& symbols) {
    if(!this->nativePath.empty()) {
        for(const auto& [key, value] : this->declarations) {
            std::string name = key.getImage();
            DynamicObject func = DynamicObject(
                VariableDeclarationExpression::loadNativeFunction(
                    this->nativePath,
                    name,
                    std::move(this->address)
                )
            );

            symbols.setSymbol(
                std::make_shared<Token>(key),
                std::move(func)
            );
        }

        return {};
    }

    for(const auto& [key, value] : this->declarations)
        symbols.setSymbol(
            std::make_shared<Token>(key),
            value->visit(symbols)
        );

    return {};
}

NativeFunction VariableDeclarationExpression::loadNativeFunction(
    std::string& libName,
    std::string& funcName,
    std::shared_ptr<Token> address
) {
    #ifndef __EMSCRIPTEN__

    void* handle;
    std::string library = N8Util::PathHelper::findSharedLibrary(
        std::string(libName.c_str())
    );

    std::filesystem::path searchPath(library);
    if(Runtime::hasLoadedLibrary(library))
        #if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
        handle = Runtime::getLoadedLibrary(library);
        #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        handle = static_cast<HMODULE>(Runtime::getLoadedLibrary(library));
        #endif
    else {
        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

        AddDllDirectory(searchPath.parent_path().wstring().c_str());
        handle = LoadLibraryA(library.c_str());

        #elif defined(__APPLE__)
        handle = dlopen(library.c_str(), RTLD_LAZY);
        #elif defined(__unix__) || defined(__linux__)
        handle = dlopen(library.c_str(), RTLD_LAZY);
        #endif

        Runtime::addLoadedLibrary(library, handle);
    }

    if(!handle) {
        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        DWORD errorMessageId = GetLastError();
        LPSTR messageBuffer = nullptr;

        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorMessageId,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR) &messageBuffer,
            0,
            NULL
        );

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);
        #endif

        throw ASTNodeException(
            std::move(address),
            "Failed to load library: " + library +
            "\r\n                 " +
            #if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
            dlerror()
            #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
            std::string(message)
            #endif
        );
    }

    std::string name = funcName;
    std::replace(name.begin(), name.end(), '.', '_');

    #if defined(__unix__) || defined(__linux__) || defined(__APPLE__)

    auto func = reinterpret_cast<NativeFunction>(dlsym(handle, name.c_str()));

    #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    auto func = std::bit_cast<NativeFunction>(GetProcAddress(
        (HMODULE) handle,
        name.c_str()
    ));

    #endif

    if(!func) {
        #if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
        dlclose(handle);
        #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        FreeLibrary((HMODULE) handle);
        #endif

        throw std::runtime_error("Failed to find function: " + funcName);
    }

    return func;

    #else
    throw std::runtime_error("Loading native functions in web mode is not supported.");
    #endif
}
