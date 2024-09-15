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

#include <ast/ASTNodeException.hpp>
#include <ast/expression/TypeExpression.hpp>

#include <parser/Token.hpp>

DynamicObject TypeExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);

    if(value.isArray())
        return std::move(DynamicObject("array"));
    else if(value.isBool())
        return std::move(DynamicObject("bool"));
    else if(value.isString())
        return std::move(DynamicObject("string"));
    else if(value.isNumber())
        return std::move(DynamicObject("number"));
    else if(value.isNil())
        return std::move(DynamicObject("nil"));
    else if(value.isFunction())
        return std::move(DynamicObject("function"));
    else if(value.isRegex())
        return std::move(DynamicObject("regex"));

    throw ASTNodeException(
        std::move(this->address),
        "Unknown dynamic object type."
    );
}
