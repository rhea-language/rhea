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

#include <N8.hpp>
#include <n8/util/Print.hpp>

#include <iostream>
#include <stdexcept>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef __EMSCRIPTEN__

extern "C" {

void executeSource(const char* sourceCode) {
    Runtime::catchSegfault();
    Runtime::execute(sourceCode);

    std::cout << std::flush;
}

}

#else

auto printBanner(N8Util::ArgumentParser argParse) -> void {
    std::cout << N8_MAIN_BANNER << std::endl;
    std::cout
        << std::endl
        << "\u001b[3;37mN8 is free software, please check the source for copying"
            << std::endl
        << "terms and conditions. This includes no warranty, not even"
            << std::endl
        << "for fitness and/or merchantability for any particular purpose."
            << std::endl;

    std::cout << std::endl
        << "\u001b[32mUsage\u001b[0m:"
        << std::endl
        << "  "
        << argParse.getProgramFileName()
        << " [args] <file-list>"
        << std::endl;
    argParse.printAllParamWithDesc();

    std::cout
        << std::endl
        << "You should have received a copy of the GNU General Public"
            << std::endl
        << "License along with N8. If not, see:"
            << std::endl
        << "<https://www.gnu.org/licenses/>.\u001b[0m"
            << std::endl << std::endl;

    #ifdef _OPENMP
    std::cout
        << "This binary is using \u001b[1mOpenMP\u001b[0m."
            << std::endl;
    #endif
}

auto main(int argc, char** argv) -> int {
    #if defined(__linux__) || defined(__APPLE__)
    Runtime::catchSegfault();
    #endif

    std::set_terminate(Runtime::terminateHandler);

    #ifdef _OPENMP
    omp_set_num_threads(
        static_cast<int>(
            std::thread::hardware_concurrency()
        )
    );
    #endif

    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(handleOut , &consoleMode);

    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;            
    SetConsoleMode(handleOut , consoleMode);
    #endif

    N8Util::ArgumentParser argParse(argc, argv);
    argParse.defineParameter("h", "help", "Show this help banner.");
    argParse.defineParameter("r", "repl", "Interative interpreter mode (REPL).");
    argParse.defineParameter("t", "test", "Run the script files in test mode.");

    if(argParse.hasParameter("h")) {
        printBanner(argParse);
        return 1;
    }

    if(argParse.hasParameter("t"))
        Runtime::setTestMode(true);

    if(argParse.hasParameter("r")) {
        Runtime::repl();
        return 0;
    }
    else if(argc > 1) {
        SymbolTable symbols;
        return Runtime::interpreter(
            symbols,
            argParse.getInputFiles()
        );
    }

    printBanner(argParse);
    return 0;
}

#endif
