/*
 * Copyright (c) 2025 - Nathanne Isip
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

#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/ast/expression/ParallelExpression.hpp>
#include <rhea/core/Runtime.hpp>
#include <rhea/core/SymbolTable.hpp>
#include <rhea/parser/LexicalAnalysisException.hpp>
#include <rhea/parser/ParserException.hpp>
#include <rhea/util/Render.hpp>
#include <rhea/util/RandomUtil.hpp>
#include <rhea/util/ThreadId.hpp>

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
    #ifndef __EMSCRIPTEN__
    }));
    #endif

    return {};
}
