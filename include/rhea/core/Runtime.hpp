/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Rhea.
 * 
 * Rhea is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Rhea is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Rhea. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RHEA_CORE_RUNTIME_HPP
#define RHEA_CORE_RUNTIME_HPP

#include <rhea/ast/ASTNode.hpp>

#include <csignal>
#include <unordered_map>

class Runtime final {
private:
    static bool testMode, unsafeMode;
    static std::vector<std::string> fileHashes;
    static std::unordered_map<std::string, void*> nativeLibraries;

public:
    static bool isTestMode();
    static void setTestMode(bool _testMode);

    static bool isUnsafeMode();
    static void setUnsafeMode(bool _unsafeMode);

    static void addLoadedLibrary(std::string libName, void* handle);
    static void* getLoadedLibrary(std::string libName);
    static bool hasLoadedLibrary(std::string libName);

    static void addFileHash(std::string hash);
    static bool hasFileHash(std::string hash);

    static void cleanUp();

    #if defined(__linux__) || defined(__APPLE__) || defined(__EMSCRIPTEN__)

    static void catchSegfault();
    static void segfaultHandler(
        int signal,
        siginfo_t *si,
        void *arg
    );

    #endif

    #ifndef __EMSCRIPTEN__

    static int interpreter(
        SymbolTable& symbols,
        std::vector<std::string> files
    );

    static void showPrompt();
    static void repl();

    #else
    static void execute(const char* sourceCode);
    #endif

    static void terminateHandler();
};

#endif
