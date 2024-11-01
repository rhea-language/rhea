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

#include <n8/ast/statement/TestStatement.hpp>
#include <n8/core/DynamicObject.hpp>
#include <n8/core/Runtime.hpp>
#include <n8/core/SymbolTable.hpp>

#include <iostream>

DynamicObject TestStatement::visit(SymbolTable& symbols) {
    if(!Runtime::isTestMode())
        return {};

    auto startTime = std::chrono::high_resolution_clock::now();
    DynamicObject value = this->testBody->visit(symbols);
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    double elapsedTimeMs = std::chrono::duration<double, std::milli>(elapsedTime).count();

    if(value.booleanEquivalent())
        std::cout << "[\u001b[1;32m SUCCESS \u001b[0m]";
    else std::cout << "[\u001b[1;31m FAILED  \u001b[0m]";

    DynamicObject name = this->testName->visit(symbols);
    std::cout << " " << elapsedTimeMs
        << " ms\t\u001b[3;97m" << name.toString()
        << "\u001b[4;0m" << std::endl;

    return {};
}
