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

#include <Zhivo.hpp>

#include <iostream>
#include <stdexcept>

auto printBanner(ZhivoUtil::ArgumentParser argParse) -> void {
    std::cout
        << "\u001b[1;36mZhivo Programming Language ("
        << ZHIVO_VERSION
        << ", "
        << ZHIVO_BUILD_TIME
        << ")\u001b[0m"
        << std::endl;

    std::cout
        << std::endl
        << "\u001b[3;37mZhivo is free software, please check the source for copying"
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
        << "License along with Zhivo. If not, see:"
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
        Runtime::cleanUp();
        std::cerr << "[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m"
            << exc.what() << "\u001b[0m" << std::endl;
    }
    catch(const ASTNodeException& nodeExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
            << "\u001b[3;37m" << nodeExc.what() << "\u001b[0m"
            << std::endl << "                 "
            << nodeExc.getAddress()->toString() << std::endl;
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        std::cerr << "[\u001b[1;31mLexical Error\u001b[0m]:" << std::endl
            << "\t" << lexAnlExc.what() << std::endl;
    }
    catch(const ParserException& parserExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        std::cerr << "[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m"
            << parserExc.what() << "\u001b[0m" << std::endl;
        std::cerr << "                 " <<
            parserExc.getAddress()->toString() << std::endl;
    }
    catch(const TerminativeBreakSignal& breakExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
            << "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
            << std::endl << "                 "
            << breakExc.getAddress().toString() << std::endl;
    }
    catch(const TerminativeContinueSignal& continueExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
            << "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
            << std::endl << "                 "
            << continueExc.getAddress().toString() << std::endl;
    }
    catch(const TerminativeReturnSignal& retExc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        std::cerr << "\u001b[0;93m"
            << retExc.getObject().toString()
            << "\u001b[0m" << std::endl;
    }
    catch(const std::exception& exc) {
        symbols.waitForThreads();
        Runtime::cleanUp();

        std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m"
            << exc.what() << "\u001b[0m" << std::endl;
    }

    return 1;
}

auto main(int argc, char** argv) -> int {
    ZhivoUtil::ArgumentParser argParse(argc, argv);
    argParse.defineParameter("h", "help", "Show this help banner.");
    argParse.defineParameter("t", "test", "Run the script files in test mode.");

    if(argParse.hasParameter("h")) {
        printBanner(argParse);
        return 1;
    }

    if(argc > 1) {
        if(argParse.hasParameter("t"))
            Runtime::setTestMode(true);

        return interpreter(argParse.getInputFiles());
    }

    printBanner(argParse);
    return 1;
}
