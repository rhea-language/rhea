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

#include <ast/ASTNode.hpp>
#include <core/Runtime.hpp>

#include <thread>
#include <vector>

#if defined(__unix__) || defined(__linux__)
#   include <dlfcn.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <Windows.h>
#else
#   error "Unsupported architecture for shared objects or dynamic libraries."
#endif

bool Runtime::testMode = false;
std::unordered_map<std::string, void*> Runtime::nativeLibraries;

bool Runtime::isTestMode() {
    return Runtime::testMode;
}

void Runtime::setTestMode(bool _testMode) {
    Runtime::testMode = _testMode;
}

void Runtime::addLoadedLibrary(std::string libName, void* handle) {
    Runtime::nativeLibraries[libName] = handle;
}

void* Runtime::getLoadedLibrary(std::string libName) {
    return Runtime::nativeLibraries[libName];
}

bool Runtime::hasLoadedLibrary(std::string libName) {
    return Runtime::nativeLibraries.find(libName) !=
        Runtime::nativeLibraries.end();
}

void Runtime::cleanUp() {
    for(const auto& [key, value] : Runtime::nativeLibraries)
        if(value != nullptr)
            #if defined(__unix__) || defined(__linux__)
            dlclose(value);
            #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
            FreeLibrary(value);
            #endif

    Runtime::nativeLibraries.clear();
}
