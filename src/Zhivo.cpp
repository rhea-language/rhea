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

#include <ast/TerminativeSignal.hpp>
#include <core/SymbolTable.hpp>
#include <parser/LexicalAnalysisException.hpp>
#include <parser/Parser.hpp>
#include <parser/ParserException.hpp>

#include <iostream>
#include <stdexcept>

auto interpreter() -> int {
    SymbolTable symbols;

    try {
        Parser parser = Parser::fromFile("test.zhv");
        parser.parse();

        for(const auto& statement : parser.getGlobalStatements())
            statement->visit(symbols);
        return 0;
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        std::cerr << "[LEXANL]:  " << lexAnlExc.what() << std::endl;
    }
    catch(const ParserException& parserExc) {
        std::cerr << "[PARSER]:  " << parserExc.what() << std::endl;
        std::cerr << "           " <<
            parserExc.getAddress()->toString() << std::endl;
    }
    catch(const TerminativeReturnSignal& retExc) {
        std::cerr << "[OBJECT]:  " <<
            retExc.getObject().toString() << std::endl;
    }
    catch(const TerminativeBreakSignal& breakExc) {
        std::cerr << "[RUNTIME]: " <<
            "Invalid break statement signal caught." << std::endl;
    }
    catch(const TerminativeContinueSignal& continueExc) {
        std::cerr << "[RUNTIME]: " <<
            "Invalid break statement signal caught." << std::endl;
    }
    catch(const std::exception& exc) {
        std::cerr << "[RUNTIME]: " << exc.what() << std::endl;
    }

    return 1;
}

decltype(interpreter()) main() {
    return interpreter();
}
