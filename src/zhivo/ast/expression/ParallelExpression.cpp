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

#include <zhivo/ast/ASTNodeException.hpp>
#include <zhivo/ast/TerminativeSignal.hpp>
#include <zhivo/ast/expression/ParallelExpression.hpp>
#include <zhivo/core/Runtime.hpp>
#include <zhivo/core/SymbolTable.hpp>
#include <zhivo/parser/LexicalAnalysisException.hpp>
#include <zhivo/parser/ParserException.hpp>
#include <zhivo/util/ThreadId.hpp>

#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

DynamicObject ParallelExpression::visit(SymbolTable& symbols) {
    std::thread task(
        std::bind([
            expr = std::move(this->expression),
            sym = &symbols
        ]() mutable {
            std::mutex mtx;

            try {
                mtx.lock();
                expr->visit(*sym);
            }
            catch(const std::system_error& exc) {
                std::cerr << "[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m"
                    << exc.what() << "\u001b[0m" << std::endl
                    << "                from thread: "
                    << ZhivoUtil::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const ASTNodeException& nodeExc) {
                sym->waitForThreads();
                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37m" << nodeExc.what() << "\u001b[0m"
                    << std::endl << "                 "
                    << nodeExc.getAddress()->toString() << std::endl
                    << "                 from thread: "
                    << ZhivoUtil::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const LexicalAnalysisException& lexAnlExc) {
                sym->waitForThreads();
                std::cerr << "[\u001b[1;31mLexical Error\u001b[0m]:" << std::endl
                    << "\t" << lexAnlExc.what() << std::endl
                    << "                 from thread: "
                    << ZhivoUtil::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const ParserException& parserExc) {
                sym->waitForThreads();
                std::cerr << "[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m"
                    << parserExc.what() << "\u001b[0m" << std::endl;
                std::cerr << "                 "
                    << parserExc.getAddress()->toString() << std::endl
                    << "                from thread: "
                    << ZhivoUtil::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const TerminativeBreakSignal& breakExc) {
                sym->waitForThreads();
                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
                    << std::endl << "                 "
                    << breakExc.getAddress().toString() << std::endl
                    << "                 from thread: "
                    << ZhivoUtil::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const TerminativeContinueSignal& continueExc) {
                sym->waitForThreads();
                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
                    << std::endl << "                 "
                    << continueExc.getAddress().toString() << std::endl
                    << "                 from thread: "
                    << ZhivoUtil::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const TerminativeReturnSignal& retExc) {
                sym->waitForThreads();
                std::cerr << "\u001b[0;93m"
                    << retExc.getObject().toString()
                    << "\u001b[0m" << std::endl;
            }
            catch(const std::exception& exc) {
                sym->waitForThreads();
                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m"
                    << exc.what() << "\u001b[0m" << std::endl
                    << "                 from thread: "
                    << ZhivoUtil::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }

            mtx.unlock();
        })
    );

    symbols.addParallelism(std::move(task));
    return {};
}
