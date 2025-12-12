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

#include <Rhea.hpp>

#include <rhea/ast/ASTNode.hpp>
#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/core/Runtime.hpp>
#include <rhea/parser/Parser.hpp>
#include <rhea/parser/ParserException.hpp>
#include <rhea/parser/Tokenizer.hpp>
#include <rhea/util/InputHighlighter.hpp>
#include <rhea/util/Render.hpp>

#include <algorithm>
#include <exception>
#include <filesystem>
#include <iostream>
#include <quickdigest5.hpp>
#include <stack>
#include <thread>
#include <vector>

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#   include <dlfcn.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <Windows.h>
#else
#   error "Unsupported architecture for shared objects or dynamic libraries."
#endif

bool Runtime::testMode = false, Runtime::unsafeMode = false;
std::unordered_map<std::string, void*> Runtime::nativeLibraries;
std::vector<std::string> Runtime::fileHashes;

bool Runtime::isTestMode() {
    return Runtime::testMode;
}

void Runtime::setTestMode(bool _testMode) {
    Runtime::testMode = _testMode;
}

bool Runtime::isUnsafeMode() {
    return Runtime::unsafeMode;
}

void Runtime::setUnsafeMode(bool _unsafeMode) {
    Runtime::unsafeMode = _unsafeMode;
}

void Runtime::addLoadedLibrary(std::string libName, void* handle) {
    #ifndef __EMSCRIPTEN__
    Runtime::nativeLibraries[libName] = handle;
    #else
    (void) libName;
    (void) handle;
    #endif
}

void* Runtime::getLoadedLibrary(std::string libName) {
    #ifndef __EMSCRIPTEN__
    auto it = Runtime::nativeLibraries.find(libName);
    return it == Runtime::nativeLibraries.end() ?
        nullptr : it->second;
    #else
    (void) libName;
    return nullptr;
    #endif
}

bool Runtime::hasLoadedLibrary(std::string libName) {
    #ifndef __EMSCRIPTEN__
    return Runtime::nativeLibraries.count(libName) == 1;
    #else
    (void) libName;
    return false;
    #endif
}

void Runtime::addFileHash(std::string hash) {
    Runtime::fileHashes.push_back(hash);
}

bool Runtime::hasFileHash(std::string hash) {
    auto begin = Runtime::fileHashes.begin(),
        end = Runtime::fileHashes.end();

    return std::find(begin, end, hash) != end;
}

void Runtime::cleanUp() {
    #ifndef __EMSCRIPTEN__
    if(Runtime::nativeLibraries.empty())
        return;

    for(const auto& [key, value] : Runtime::nativeLibraries)
        if(value != nullptr)
            #if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
            dlclose(value);
            #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
            FreeLibrary((HMODULE) value);
            #endif

    Runtime::nativeLibraries.clear();
    #endif

}

#ifndef __EMSCRIPTEN__
int Runtime::interpreter(SymbolTable& symbols, std::vector<std::string> files) {
    try {
        std::vector<std::string>::iterator iterator;

        parsync(iterator = files.begin(); iterator != files.end(); iterator++) {
            std::string fileHash = QuickDigest5::fileToHash(*iterator);
            if(Runtime::hasFileHash(fileHash))
                continue;
            Runtime::addFileHash(fileHash);

            Parser parser = Parser::fromFile(*iterator);
            parser.parse();

            for(const auto& statement : parser.getGlobalStatements())
                statement->visit(symbols);
        }

        return 0;
    }
    catch(const std::system_error& exc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(exc.what());
        RheaUtil::renderError("\u001b[0m\r\n");
    }
    catch(const ASTNodeException& nodeExc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(nodeExc.what());
        RheaUtil::renderError("\u001b[0m\r\n                 ");
        RheaUtil::renderError(nodeExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
        RheaUtil::renderError(lexAnlExc.what());
        RheaUtil::renderError("\r\n");
    }
    catch(const ParserException& parserExc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
        RheaUtil::renderError(parserExc.what());
        RheaUtil::renderError("\u001b[0m\r\n                 ");
        RheaUtil::renderError(parserExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeBreakSignal& breakExc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        RheaUtil::renderError(breakExc.getAddress().toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeContinueSignal& continueExc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        RheaUtil::renderError(continueExc.getAddress().toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeReturnSignal& retExc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("\u001b[0;93m");
        RheaUtil::renderError(retExc.getObject().toString());
        RheaUtil::renderError("\u001b[0m\r\n");
    }
    catch(const TerminativeThrowSignal& throwExc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(throwExc.getObject().toString());
        RheaUtil::renderError("\u001b[0m\r\n                  ");
        RheaUtil::renderError(throwExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const std::exception& exc) {
        symbols.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(exc.what());
        RheaUtil::renderError("\u001b[0m\r\n");
    }

    return 1;
}

void Runtime::showPrompt() {
    std::string workingDir = std::filesystem::current_path().string();
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime;

    #ifdef _WIN32
    localtime_s(&localTime, &currentTime);
    #else
    localtime_r(&currentTime, &localTime);
    #endif

    std::cout << std::endl;
    std::cout << "\u001b[1;34m\u250c─\u001b[0m [ \u001b[3;33m" << workingDir << "\u001b[0m ]" << std::endl;
    std::cout << "\u001b[1;34m\u251c─\u001b[0m [ \u001b[33m" << std::put_time(&localTime, "%m/%d/%Y %H:%M:%S") << "\u001b[0m ]" << std::endl;
}

void Runtime::repl() {
    SymbolTable symtab;
    RheaUtil::InputHighlighter inputHighlighter(
        std::string("\u001b[1;34m\u2514──\u2a65\u001b[0m "),
        OperatorsAndKeys::keywords
    );

    std::string input, line;
    int iterNum = 1;

    std::cout << RHEA_MAIN_BANNER << std::endl;
    Runtime::showPrompt();

    while(true) {
        input = inputHighlighter.readInput();
        input += line + '\n';

        try{
            Tokenizer tokenizer(
                input,
                "<repl, iteration: " +
                    std::to_string(iterNum) + ">"
            );
            tokenizer.scan();

            Parser parser(tokenizer.getTokens());
            parser.parse();

            for(const auto& statement : parser.getGlobalStatements())
                statement->visit(symtab);
        }
        catch(const std::system_error& exc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
            RheaUtil::renderError(exc.what());
            RheaUtil::renderError("\u001b[0m\r\n");
        }
        catch(const ASTNodeException& nodeExc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
            RheaUtil::renderError(nodeExc.what());
            RheaUtil::renderError("\u001b[0m\r\n                 ");
            RheaUtil::renderError(nodeExc.getAddress()->toString());
            RheaUtil::renderError("\r\n");
        }
        catch(const LexicalAnalysisException& lexAnlExc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
            RheaUtil::renderError(lexAnlExc.what());
            RheaUtil::renderError("\r\n");
        }
        catch(const ParserException& parserExc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
            RheaUtil::renderError(parserExc.what());
            RheaUtil::renderError("\u001b[0m\r\n                 ");
            RheaUtil::renderError(parserExc.getAddress()->toString());
            RheaUtil::renderError("\r\n");
        }
        catch(const TerminativeBreakSignal& breakExc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError(
                "[\u001b[1;31mRuntime Error\u001b[0m]: "
                "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
                "\r\n                 "
            );
            RheaUtil::renderError(breakExc.getAddress().toString());
            RheaUtil::renderError("\r\n");
        }
        catch(const TerminativeContinueSignal& continueExc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError(
                "[\u001b[1;31mRuntime Error\u001b[0m]: "
                "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
                "\r\n                 "
            );
            RheaUtil::renderError(continueExc.getAddress().toString());
            RheaUtil::renderError("\r\n");
        }
        catch(const TerminativeReturnSignal& retExc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError("\u001b[0;93m");
            RheaUtil::renderError(retExc.getObject().toString());
            RheaUtil::renderError("\u001b[0m\r\n");
        }
        catch(const TerminativeThrowSignal& throwExc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
            RheaUtil::renderError(throwExc.getObject().toString());
            RheaUtil::renderError("\u001b[0m\r\n                  ");
            RheaUtil::renderError(throwExc.getAddress()->toString());
            RheaUtil::renderError("\r\n");
        }
        catch(const std::exception& exc) {
            symtab.waitForTasks();
            Runtime::cleanUp();

            RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
            RheaUtil::renderError(exc.what());
            RheaUtil::renderError("\u001b[0m\r\n");
        }

        input.clear();
        iterNum++;

        Runtime::showPrompt();
    }
}

#else

void Runtime::execute(const char* sourceCode) {
    SymbolTable symtab;
    try{
        Tokenizer tokenizer(std::string(sourceCode), "<exec_engine>");
        tokenizer.scan();

        Parser parser(tokenizer.getTokens());
        parser.parse();

        for(const auto& statement : parser.getGlobalStatements())
            statement->visit(symtab);
    }
    catch(const std::system_error& exc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(exc.what());
        RheaUtil::renderError("\u001b[0m\r\n");
    }
    catch(const ASTNodeException& nodeExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(nodeExc.what());
        RheaUtil::renderError("\u001b[0m\r\n                 ");
        RheaUtil::renderError(nodeExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
        RheaUtil::renderError(lexAnlExc.what());
        RheaUtil::renderError("\r\n");
    }
    catch(const ParserException& parserExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
        RheaUtil::renderError(parserExc.what());
        RheaUtil::renderError("\u001b[0m\r\n                 ");
        RheaUtil::renderError(parserExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeBreakSignal& breakExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        RheaUtil::renderError(breakExc.getAddress().toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeContinueSignal& continueExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        RheaUtil::renderError(continueExc.getAddress().toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeReturnSignal& retExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("\u001b[0;93m");
        RheaUtil::renderError(retExc.getObject().toString());
        RheaUtil::renderError("\u001b[0m\r\n");
    }
    catch(const TerminativeThrowSignal& throwExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(throwExc.getObject().toString());
        RheaUtil::renderError("\u001b[0m\r\n                  ");
        RheaUtil::renderError(throwExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const std::exception& exc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(exc.what());
        RheaUtil::renderError("\u001b[0m\r\n");
    }
}

#endif

#if defined(__linux__) || defined(__APPLE__) || defined(__EMSCRIPTEN__)

void Runtime::segfaultHandler(
    int signal, siginfo_t *si,
    void *arg __attribute__((unused))
) {
    std::cout.sync_with_stdio(true);
    std::cout << std::endl;

    std::cout << "\u001b[1;41m     Segmentation Fault     \u001b[0m"
              << std::endl;
    std::cout << "\u001b[1;31mCaught signal\u001b[0m:\t\t0x"
              << std::hex << std::setw(2) << std::setfill('0') << signal << std::dec << "\n";
    std::cout << "\u001b[1;31mFaulting address\u001b[0m:\t0x"
              << std::hex << std::setw(2) << std::setfill('0')
              << reinterpret_cast<std::uintptr_t>(si->si_addr)
              << std::dec << "\n";
    std::cout << "\u001b[1;31mSignal code\u001b[0m:\t\t"
              << si->si_code << "\n";
    std::cout << "\u001b[1;31mSending process ID\u001b[0m:\t"
              << si->si_pid << "\n";
    std::cout << "\u001b[1;31mReal user ID\u001b[0m:\t\t"
              << si->si_uid << "\n";
    std::cout << "\u001b[1;31mExit value\u001b[0m:\t\t"
              << si->si_status << "\n";
    std::cout << "\u001b[1;31mBand event\u001b[0m:\t\t"
              << si->si_band << "\n";
    std::cout << "\u001b[1;31mFaulting address\u001b[0m:\t"
              << si->si_addr << "\n";

    #ifndef __APPLE__
    std::cout << "\u001b[1;31mTimer overrun\u001b[0m:\t\t"
              << si->si_overrun << "\n";
    std::cout << "\u001b[1;31mTimer ID\u001b[0m:\t\t"
              << si->si_timerid << "\n";
    std::cout << "\u001b[1;31mFile descriptor\u001b[0m:\t"
              << si->si_fd << "\n";
    #endif

    std::exit(si->si_status);
}

void Runtime::catchSegfault() {
    struct sigaction sa;

    std::memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = Runtime::segfaultHandler;
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);
}

#endif

void Runtime::terminateHandler() {
    std::exception_ptr eptr = std::current_exception();
    try {
        std::rethrow_exception(eptr);
    }
    catch(const std::system_error& exc) {
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(exc.what());
        RheaUtil::renderError("\u001b[0m\r\n");
    }
    catch(const ASTNodeException& nodeExc) {
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(nodeExc.what());
        RheaUtil::renderError("\u001b[0m\r\n                 ");
        RheaUtil::renderError(nodeExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
        RheaUtil::renderError(lexAnlExc.what());
        RheaUtil::renderError("\r\n");
    }
    catch(const ParserException& parserExc) {
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
        RheaUtil::renderError(parserExc.what());
        RheaUtil::renderError("\u001b[0m\r\n                 ");
        RheaUtil::renderError(parserExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeBreakSignal& breakExc) {
        Runtime::cleanUp();

        RheaUtil::renderError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        RheaUtil::renderError(breakExc.getAddress().toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeContinueSignal& continueExc) {
        Runtime::cleanUp();

        RheaUtil::renderError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        RheaUtil::renderError(continueExc.getAddress().toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const TerminativeReturnSignal& retExc) {
        Runtime::cleanUp();

        RheaUtil::renderError("\u001b[0;93m");
        RheaUtil::renderError(retExc.getObject().toString());
        RheaUtil::renderError("\u001b[0m\r\n");
    }
    catch(const TerminativeThrowSignal& throwExc) {
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(throwExc.getObject().toString());
        RheaUtil::renderError("\u001b[0m\r\n                  ");
        RheaUtil::renderError(throwExc.getAddress()->toString());
        RheaUtil::renderError("\r\n");
    }
    catch(const std::exception& exc) {
        Runtime::cleanUp();

        RheaUtil::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        RheaUtil::renderError(exc.what());
        RheaUtil::renderError("\u001b[0m\r\n");
    }

    std::abort();
}
