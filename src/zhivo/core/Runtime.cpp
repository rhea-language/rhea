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

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/ast/ASTNodeException.hpp>
#include <zhivo/ast/TerminativeSignal.hpp>
#include <zhivo/core/Runtime.hpp>
#include <zhivo/parser/Parser.hpp>
#include <zhivo/parser/ParserException.hpp>
#include <zhivo/parser/Tokenizer.hpp>

#include <exception>
#include <iostream>
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
            #if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
            dlclose(value);
            #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
            FreeLibrary((HMODULE) value);
            #endif

    Runtime::nativeLibraries.clear();
}

static inline bool isBalanced(const std::string& input) {
    std::stack<char> stack;

    for(char ch : input) {
        if(ch == '(' || ch == '{' || ch == '[')
            stack.push(ch);
        else if(ch == ')' || ch == '}' || ch == ']') {
            if(stack.empty())
                return false;

            char top = stack.top();
            if((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '['))
                return false;

            stack.pop();
        }
    }

    return stack.empty();
}

void Runtime::repl() {
    SymbolTable symtab;
    std::string input, line;
    int iterNum = 1;

    std::cout << ">>> ";
    while(std::getline(std::cin, line)) {
        input += line + '\n';

        if(isBalanced(input)) {
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

                symtab.detachParallelNodes();
            }
            catch(const std::system_error& exc) {
                Runtime::cleanUp();
                std::cerr << "[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m"
                    << exc.what() << "\u001b[0m" << std::endl;
            }
            catch(const ASTNodeException& nodeExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37m" << nodeExc.what() << "\u001b[0m"
                    << std::endl << "                 "
                    << nodeExc.getAddress()->toString() << std::endl;
            }
            catch(const LexicalAnalysisException& lexAnlExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mLexical Error\u001b[0m]:" << std::endl
                    << "\t" << lexAnlExc.what() << std::endl;
            }
            catch(const ParserException& parserExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m"
                    << parserExc.what() << "\u001b[0m" << std::endl;
                std::cerr << "                 " <<
                    parserExc.getAddress()->toString() << std::endl;
            }
            catch(const TerminativeBreakSignal& breakExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
                    << std::endl << "                 "
                    << breakExc.getAddress().toString() << std::endl;
            }
            catch(const TerminativeContinueSignal& continueExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
                    << std::endl << "                 "
                    << continueExc.getAddress().toString() << std::endl;
            }
            catch(const TerminativeReturnSignal& retExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                std::cerr << "\u001b[0;93m"
                    << retExc.getObject().toString()
                    << "\u001b[0m" << std::endl;
            }
            catch(const std::exception& exc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m"
                    << exc.what() << "\u001b[0m" << std::endl;
            }

            input.clear();
            iterNum++;

            std::cout << std::endl << ">>> ";
        }
        else std::cout << "... ";
    }
}
