/*
 * Copyright (c) 2025 - Nathanne Isip
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
#include <n8/util/Render.hpp>
#include <n8/util/RandomUtil.hpp>
#include <n8/util/ThreadId.hpp>

#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

DynamicObject ParallelExpression::visit(SymbolTable& symbols) {
    #ifndef __EMSCRIPTEN__
    symbols.addParallelism(std::async(std::launch::async, [
        expr = this->expression,
        symbols
    ]() mutable {
    #endif
        try {
            #ifndef __EMSCRIPTEN__
            expr->visit(symbols);
            #else
            this->expression->visit(symbols);
            #endif
        }
        catch(const std::system_error& exc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
            N8Util::renderError(exc.what());
            N8Util::renderError("\u001b[0m\r\n");
        }
        catch(const ASTNodeException& nodeExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
            N8Util::renderError(nodeExc.what());
            N8Util::renderError("\u001b[0m\r\n                 ");
            N8Util::renderError(nodeExc.getAddress()->toString());
            N8Util::renderError("\r\n");
        }
        catch(const LexicalAnalysisException& lexAnlExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
            N8Util::renderError(lexAnlExc.what());
            N8Util::renderError("\r\n");
        }
        catch(const ParserException& parserExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
            N8Util::renderError(parserExc.what());
            N8Util::renderError("\u001b[0m\r\n                 ");
            N8Util::renderError(parserExc.getAddress()->toString());
            N8Util::renderError("\r\n");
        }
        catch(const TerminativeBreakSignal& breakExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError(
                "[\u001b[1;31mRuntime Error\u001b[0m]: "
                "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
                "\r\n                 "
            );
            N8Util::renderError(breakExc.getAddress().toString());
            N8Util::renderError("\r\n");
        }
        catch(const TerminativeContinueSignal& continueExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError(
                "[\u001b[1;31mRuntime Error\u001b[0m]: "
                "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
                "\r\n                 "
            );
            N8Util::renderError(continueExc.getAddress().toString());
            N8Util::renderError("\r\n");
        }
        catch(const TerminativeReturnSignal& retExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError("\u001b[0;93m");
            N8Util::renderError(retExc.getObject().toString());
            N8Util::renderError("\u001b[0m\r\n");
        }
        catch(const TerminativeThrowSignal& throwExc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
            N8Util::renderError(throwExc.getObject().toString());
            N8Util::renderError("\u001b[0m\r\n                  ");
            N8Util::renderError(throwExc.getAddress()->toString());
            N8Util::renderError("\r\n");
        }
        catch(const std::exception& exc) {
            symbols.waitForTasks();
            Runtime::cleanUp();

            N8Util::renderError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
            N8Util::renderError(exc.what());
            N8Util::renderError("\u001b[0m\r\n");
        }
    #ifndef __EMSCRIPTEN__
    }));
    #endif

    return {};
}
