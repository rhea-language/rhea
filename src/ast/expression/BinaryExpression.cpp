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

#include <ast/expression/BinaryExpression.hpp>
#include <memory>

DynamicObject BinaryExpression::visit(SymbolTable& symbols) {
    DynamicObject lValue = this->left->visit(symbols);
    DynamicObject rValue = this->right->visit(symbols);

    if(lValue.isNumber() && rValue.isNumber())
        return applyNumOp(lValue, rValue);
    else if(lValue.isString() || rValue.isString())
        return applyStringOp(lValue, rValue);
    else if(lValue.isBool() || rValue.isBool())
        return applyBoolOp(lValue, rValue);

    throw std::runtime_error("Unsupported operation for these types");
    return {};
}

DynamicObject BinaryExpression::applyNumOp(DynamicObject& lValue, DynamicObject& rValue) {
    if(this->op == "+")
        return DynamicObject(lValue.getNumber() + rValue.getNumber());
    else if(this->op == "-")
        return DynamicObject(lValue.getNumber() - rValue.getNumber());
    else if(this->op == "*")
        return DynamicObject(lValue.getNumber() * rValue.getNumber());
    else if(this->op == "/")
        return DynamicObject(lValue.getNumber() / rValue.getNumber());
    else if(this->op == "%")
        return DynamicObject((float) ((int) lValue.getNumber() % (int) rValue.getNumber()));
    else if(this->op == "&")
        return DynamicObject((float) ((int) lValue.getNumber() & (int) rValue.getNumber()));
    else if(this->op == "|")
        return DynamicObject((float) ((int) lValue.getNumber() | (int) rValue.getNumber()));
    else if(this->op == ">")
        return DynamicObject(lValue.getNumber() > rValue.getNumber());
    else if(this->op == "<")
        return DynamicObject(lValue.getNumber() < rValue.getNumber());
    else if(this->op == ">=")
        return DynamicObject(lValue.getNumber() >= rValue.getNumber());
    else if(this->op == "<=")
        return DynamicObject(lValue.getNumber() <= rValue.getNumber());
    else if(this->op == "==")
        return DynamicObject(std::fabs(lValue.getNumber() - rValue.getNumber()) < __DBL_EPSILON__);

    throw std::runtime_error("Unknown operator");
}

DynamicObject BinaryExpression::applyStringOp(DynamicObject& lValue, DynamicObject& rValue) {
    if(this->op == "+")
        return DynamicObject(lValue.toString() + rValue.toString());
    else if(this->op == "-")
        return DynamicObject(std::regex_replace(
            lValue.toString(),
            std::regex(rValue.toString()),
            ""
        ));

    throw std::runtime_error("Unknown operator for string");
}

DynamicObject BinaryExpression::applyBoolOp(DynamicObject& lValue, DynamicObject& rValue) {
    if(this->op == "||")
        return DynamicObject(lValue.getBool() || rValue.getBool());
    else if(this->op == "&&")
        return DynamicObject(lValue.getBool() && rValue.getBool());

    throw std::runtime_error("Unsupported operation for boolean");
}
