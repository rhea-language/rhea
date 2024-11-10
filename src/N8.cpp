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

auto printBanner(N8Util::ArgumentParser argParse) -> void {
    std::cout
        << "\u001b[1;36mN8 Programming Language ("
        << N8_VERSION
        << ", "
        << N8_BUILD_TIME
        << ")\u001b[0m"
        << std::endl;

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
            << std::endl;
}

auto interpreter(std::vector<std::string> files) -> int {
    SymbolTable symbols;

    try {
        std::vector<std::string>::iterator iterator;

        for(iterator = files.begin(); iterator != files.end(); iterator++) {
            Parser parser = Parser::fromFile(*iterator);
            parser.parse();

            for(const auto& statement : parser.getGlobalStatements())
                statement->visit(symbols);
        }

        symbols.detachParallelNodes();
        return 0;
    }
    catch(const std::system_error& exc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(exc.what());
        N8Util::printError("\u001b[0m\r\n");
    }
    catch(const ASTNodeException& nodeExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(nodeExc.what());
        N8Util::printError("\u001b[0m\r\n                 ");
        N8Util::printError(nodeExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
        N8Util::printError(lexAnlExc.what());
        N8Util::printError("\r\n");
    }
    catch(const ParserException& parserExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
        N8Util::printError(parserExc.what());
        N8Util::printError("\u001b[0m\r\n                 ");
        N8Util::printError(parserExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeBreakSignal& breakExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        N8Util::printError(breakExc.getAddress().toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeContinueSignal& continueExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        N8Util::printError(continueExc.getAddress().toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeReturnSignal& retExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("\u001b[0;93m");
        N8Util::printError(retExc.getObject().toString());
        N8Util::printError("\u001b[0m\r\n");
    }
    catch(const TerminativeThrowSignal& throwExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(throwExc.getObject().toString());
        N8Util::printError("\u001b[0m\r\n                  ");
        N8Util::printError(throwExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const std::exception& exc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(exc.what());
        N8Util::printError("\u001b[0m\r\n");
    }

    return 1;
}

#ifndef __EMSCRIPTEN__

auto main(int argc, char** argv) -> int {
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
    else if(argc > 1)
        return interpreter(argParse.getInputFiles());

    printBanner(argParse);
    return 0;
}

#else

extern "C" {

void executeSource(const char* outputElement, const char* sourceCode) {
    Runtime::setOutputElementId(std::string(outputElement));
    Runtime::execute(sourceCode);
}

}

#endif
