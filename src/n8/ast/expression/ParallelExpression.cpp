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

#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/TerminativeSignal.hpp>
#include <n8/ast/expression/ParallelExpression.hpp>
#include <n8/core/Runtime.hpp>
#include <n8/core/SymbolTable.hpp>
#include <n8/parser/LexicalAnalysisException.hpp>
#include <n8/parser/ParserException.hpp>
#include <n8/util/Print.hpp>
#include <n8/util/RandomUtil.hpp>
#include <n8/util/ThreadId.hpp>

#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

DynamicObject ParallelExpression::visit(SymbolTable& symbols) {
    symbols.addParallelism(std::async(std::launch::async, [
        expr = this->expression,
        symbols
    ]() mutable {
        try {
            expr->visit(symbols);
        }
        catch(const std::system_error& exc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::printError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
            N8Util::printError(exc.what());
            N8Util::printError("\u001b[0m\r\n");
        }
        catch(const ASTNodeException& nodeExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
            N8Util::printError(nodeExc.what());
            N8Util::printError("\u001b[0m\r\n                 ");
            N8Util::printError(nodeExc.getAddress()->toString());
            N8Util::printError("\r\n");
        }
        catch(const LexicalAnalysisException& lexAnlExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::printError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
            N8Util::printError(lexAnlExc.what());
            N8Util::printError("\r\n");
        }
        catch(const ParserException& parserExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::printError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
            N8Util::printError(parserExc.what());
            N8Util::printError("\u001b[0m\r\n                 ");
            N8Util::printError(parserExc.getAddress()->toString());
            N8Util::printError("\r\n");
        }
        catch(const TerminativeBreakSignal& breakExc) {
            symbols.waitForTasks();
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
            symbols.waitForTasks();
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
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::printError("\u001b[0;93m");
            N8Util::printError(retExc.getObject().toString());
            N8Util::printError("\u001b[0m\r\n");
        }
        catch(const TerminativeThrowSignal& throwExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::printError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
            N8Util::printError(throwExc.getObject().toString());
            N8Util::printError("\u001b[0m\r\n                  ");
            N8Util::printError(throwExc.getAddress()->toString());
            N8Util::printError("\r\n");
        }
        catch(const std::exception& exc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
            N8Util::printError(exc.what());
            N8Util::printError("\u001b[0m\r\n");
        }
    }));

    return {};
}
