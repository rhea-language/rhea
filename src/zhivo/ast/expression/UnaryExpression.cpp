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
#include <zhivo/ast/expression/UnaryExpression.hpp>
#include <zhivo/parser/Token.hpp>

#include <algorithm>

DynamicObject UnaryExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);

    if(this->op == "!")
        return DynamicObject(!value.booleanEquivalent());
    else if(value.isArray() && this->op == "~") {
        std::vector<DynamicObject> objects = *value.getArray();
        std::reverse(objects.begin(), objects.end());

        return DynamicObject(
            std::make_shared<std::vector<DynamicObject>>(objects)
        );
    }
    else if(value.isNumber()) {
        if(this->op == "+")
            return DynamicObject(+value.getNumber());
        else if(this->op == "-")
            return DynamicObject(-value.getNumber());
        else if(this->op == "~")
            return DynamicObject(
                static_cast<double>(
                    ~static_cast<long>(
                        value.getNumber()
                    )
                )
            );
    }
    else if(value.isString()) {
        if(this->op == "*")
            return DynamicObject((double) value.getString().length());
        else if(this->op == "~") {
            std::string str = value.getString();
            std::reverse(str.begin(), str.end());

            return DynamicObject(std::move(str));
        }
    }

    #ifdef _MSC_VER
    #   pragma warning(disable : 5272)
    #endif
    throw ASTNodeException(
        std::move(this->address),
        "Invalid unary expression operation"
    );
}
