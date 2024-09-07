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

#include <ast/expression/UnaryExpression.hpp>

DynamicObject UnaryExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);
    if(value.isNumber()) {
        if(this->op == "+")
            return DynamicObject(+value.getNumber());
        else if(this->op == "-")
            return DynamicObject(-value.getNumber());
        else if(this->op == "~")
            return DynamicObject((double) ~((unsigned long) value.getNumber()));
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
    else if(value.isBool() && this->op == "!")
        return DynamicObject(!value.getBool());

    throw std::runtime_error("Invalid unary expression operation");
}
