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
#include <n8/util/ThreadId.hpp>

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
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m"
                    << exc.what() << "\u001b[0m" << std::endl
                    << "                from thread: "
                    << N8Util::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const ASTNodeException& nodeExc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37m" << nodeExc.what() << "\u001b[0m"
                    << std::endl << "                 "
                    << nodeExc.getAddress()->toString() << std::endl
                    << "                 from thread: "
                    << N8Util::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const LexicalAnalysisException& lexAnlExc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mLexical Error\u001b[0m]:" << std::endl
                    << "\t" << lexAnlExc.what() << std::endl
                    << "                 from thread: "
                    << N8Util::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const ParserException& parserExc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m"
                    << parserExc.what() << "\u001b[0m" << std::endl;
                std::cerr << "                 "
                    << parserExc.getAddress()->toString() << std::endl
                    << "                from thread: "
                    << N8Util::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const TerminativeBreakSignal& breakExc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
                    << std::endl << "                 "
                    << breakExc.getAddress().toString() << std::endl
                    << "                 from thread: "
                    << N8Util::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const TerminativeContinueSignal& continueExc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    << "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
                    << std::endl << "                 "
                    << continueExc.getAddress().toString() << std::endl
                    << "                 from thread: "
                    << N8Util::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }
            catch(const TerminativeReturnSignal& retExc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "\u001b[0;93m"
                    << retExc.getObject().toString()
                    << "\u001b[0m" << std::endl;
            }
            catch(const TerminativeThrowSignal& throwExc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mUncaught Error\u001b[0m]: "
                    << "\u001b[3;37m"
                    << throwExc.getObject().toString()
                    << "\u001b[0m"
                    << std::endl << "                  "
                    << throwExc.getAddress()->toString() << std::endl;
            }
            catch(const std::exception& exc) {
                sym->waitForThreads();
                Runtime::cleanUp();

                std::cerr << "[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m"
                    << exc.what() << "\u001b[0m" << std::endl
                    << "                 from thread: "
                    << N8Util::getThreadId(std::this_thread::get_id())
                    << std::endl;
            }

            mtx.unlock();
        })
    );

    symbols.addParallelism(std::move(task));
    return {};
}
