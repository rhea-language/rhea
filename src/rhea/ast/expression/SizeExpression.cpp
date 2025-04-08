/*
 * Copyright (c) 2024 - Nathanne Isip
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
#include <rhea/ast/expression/SizeExpression.hpp>

#include <rhea/parser/Token.hpp>

DynamicObject SizeExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);
    if(value.isArray())
        return DynamicObject((float) value.getArray()->size());
    else if(value.isBool() || value.isNumber())
        return DynamicObject(1.0f);
    else if(value.isRegex())
        return DynamicObject((float) value.getRegex()->getPattern().size());
    else if(value.isString())
        return DynamicObject((float) value.getString().size());

    return DynamicObject(0.0f);
}
