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
#include <ast/expression/ArrayAccessExpression.hpp>
#include <ast/expression/BinaryExpression.hpp>
#include <ast/expression/VariableAccessExpression.hpp>

#include <parser/Token.hpp>

#include <memory>

DynamicObject BinaryExpression::visit(SymbolTable& symbols) {
    if(auto* varAccess = dynamic_cast<VariableAccessExpression*>(this->left.get())) {
        DynamicObject rValue = this->right->visit(symbols);
        symbols.setSymbol(varAccess->getName().getImage(), rValue);

        return rValue;
    }
    else if(auto* arrayAccess = dynamic_cast<ArrayAccessExpression*>(this->left.get())) {
        auto arrayExpr = arrayAccess->getArrayExpression();
        auto arrayIdx = arrayAccess->getIndexExpression();

        DynamicObject arrayVal = arrayExpr->visit(symbols);
        if(!arrayVal.isArray())
            throw ASTNodeException(
                std::move(this->address),
                "Error updating array element."
            );

        DynamicObject indexVal = arrayIdx->visit(symbols);
        if(!indexVal.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Error updating array element."
            );

        DynamicObject rValue = this->right->visit(symbols);
        std::unique_ptr<DynamicObject> rValuePtr = std::make_unique<DynamicObject>(rValue);

        arrayVal.setArrayElement((int) indexVal.getNumber(), std::move(rValuePtr));
        return arrayVal;
    }

    DynamicObject lValue = this->left->visit(symbols);
    DynamicObject rValue = this->right->visit(symbols);

    if(lValue.isNumber() && rValue.isNumber())
        return applyNumOp(lValue, rValue);
    else if(lValue.isString() || rValue.isString())
        return applyStringOp(lValue, rValue);
    else if(lValue.isBool() || rValue.isBool())
        return applyBoolOp(lValue, rValue);

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for these types"
    );

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

    throw ASTNodeException(
        std::move(this->address),
        "Unknown operator"
    );
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

    throw ASTNodeException(
        std::move(this->address),
        "Unknown operator for string"
    );
}

DynamicObject BinaryExpression::applyBoolOp(DynamicObject& lValue, DynamicObject& rValue) {
    if(this->op == "||")
        return DynamicObject(lValue.getBool() || rValue.getBool());
    else if(this->op == "&&")
        return DynamicObject(lValue.getBool() && rValue.getBool());

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for boolean"
    );
}
