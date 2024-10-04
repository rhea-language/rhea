/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Zhivo.
 * 
 * Zhivo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Zhivo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Zhivo. If not, see <https://www.gnu.org/licenses/>.
 */

#include <ast/ASTNodeException.hpp>
#include <ast/statement/VariableDeclarationStatement.hpp>
#include <core/Runtime.hpp>
#include <core/SymbolTable.hpp>

#include <dlfcn.h>

DynamicObject VariableDeclarationStatement::visit(SymbolTable& symbols) {
    if(!this->nativePath.empty()) {
        for(const auto& [key, value] : this->declarations) {
            std::string name = key.getImage();

            symbols.setSymbol(name, DynamicObject(
                VariableDeclarationStatement::loadNativeFunction(
                    this->nativePath,
                    name,
                    std::move(this->address)
                )
            ));
        }

        return {};
    }

    for(const auto& [key, value] : this->declarations)
        symbols.setSymbol(
            key.getImage(),
            value->visit(symbols)
        );

    return {};
}

NativeFunction VariableDeclarationStatement::loadNativeFunction(
    std::string& libName,
    std::string& funcName,
    std::unique_ptr<Token> address
) {
    void* handle;
    if(Runtime::hasLoadedLibrary(libName))
        handle = Runtime::getLoadedLibrary(libName);
    else {
        handle = dlopen(libName.c_str(), RTLD_LAZY);
        Runtime::addLoadedLibrary(libName, handle);
    }

    if(!handle)
        throw ASTNodeException(
            std::move(address),
            "Failed to load library: " + libName +
            "\r\n                 " + dlerror()
        );

    auto func = reinterpret_cast<NativeFunction>(dlsym(handle, funcName.c_str()));
    if(!func) {
        dlclose(handle);
        throw std::runtime_error("Failed to find function: " + funcName);
    }

    return func;
}
