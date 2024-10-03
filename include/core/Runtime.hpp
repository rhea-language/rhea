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

#ifndef ZHIVO_CORE_RUNTIME_HPP
#define ZHIVO_CORE_RUNTIME_HPP

#include <ast/ASTNode.hpp>

#include <unordered_map>

class Runtime {
private:
    static bool testMode;
    static std::unordered_map<std::string, void*> nativeLibraries;

public:
    static bool isTestMode();
    static void setTestMode(bool _testMode);

    static void addLoadedLibrary(std::string libName, void* handle);
    static void* getLoadedLibrary(std::string libName);
    static bool hasLoadedLibrary(std::string libName);

    static void cleanUp();
};

#endif
