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

#include <rhea/ast/expression/FunctionDeclarationExpression.hpp>
#include <rhea/ast/expression/RenderExpression.hpp>
#include <rhea/util/Render.hpp>
#include <iostream>

DynamicObject RenderExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);
    std::string str = value.toString();

    if(this->errorStream)
        RheaUtil::renderError(str);
    else RheaUtil::render(str);

    if(this->newLine) {
        if(this->errorStream)
            RheaUtil::renderError("\r\n");
        else RheaUtil::render("\r\n");
    }

    return value;
}
