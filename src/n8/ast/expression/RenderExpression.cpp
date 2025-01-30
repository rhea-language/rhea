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

#include <n8/ast/expression/FunctionDeclarationExpression.hpp>
#include <n8/ast/expression/RenderExpression.hpp>
#include <n8/util/Render.hpp>
#include <iostream>

DynamicObject RenderExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);
    std::string str = value.toString();

    if(this->errorStream)
        N8Util::renderError(str);
    else N8Util::render(str);

    if(this->newLine) {
        if(this->errorStream)
            N8Util::renderError("\r\n");
        else N8Util::render("\r\n");
    }

    return value;
}
