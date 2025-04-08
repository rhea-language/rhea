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

#include <Rhea.hpp>
#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/ast/expression/ArrayAccessExpression.hpp>
#include <rhea/ast/expression/BinaryExpression.hpp>
#include <rhea/ast/expression/VariableAccessExpression.hpp>
#include <rhea/parser/Token.hpp>
#include <rhea/util/VectorMath.hpp>

#include <limits>
#include <memory>
#include <regex>

DynamicObject BinaryExpression::visit(SymbolTable& symbols) {
    auto* arrayAccess = dynamic_cast<ArrayAccessExpression*>(this->left.get());
    if(arrayAccess && this->op == "=") {
        auto arrayExpr = arrayAccess->getArrayExpression();
        auto arrayIdx = arrayAccess->getIndexExpression();

        DynamicObject arrayVal = arrayExpr->visit(symbols);
        if(!arrayVal.isArray())
            throw ASTNodeException(
                std::move(this->address),
                "Object is not an array, cannot update value in specified index."
            );

        DynamicObject indexVal = arrayIdx->visit(symbols);
        if(!indexVal.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Specified index is not a number."
            );

        DynamicObject rValue = this->right->visit(symbols);
        std::shared_ptr<DynamicObject> rValuePtr = std::make_shared<DynamicObject>(rValue);

        arrayVal.setArrayElement(
            std::move(this->address),
            (size_t) indexVal.getNumber(),
            std::move(rValuePtr)
        );

        return arrayVal;
    }

    auto* varAccess = dynamic_cast<VariableAccessExpression*>(this->left.get());
    if(varAccess && this->op == "=") {
        DynamicObject value = this->right->visit(symbols);
        symbols.setSymbol(
            std::make_shared<Token>(varAccess->getName()),
            value
        );

        return value;
    }

    DynamicObject lValue = this->left->visit(symbols);
    DynamicObject rValue = this->right->visit(symbols);

    if(this->op == "+")
        return lValue + rValue;
    else if(this->op == "-")
        return lValue - rValue;
    else if(this->op == "/")
        return lValue / rValue;
    else if(this->op == "\\")
        return rValue / lValue;
    else if(this->op == "*")
        return lValue * rValue;
    else if(this->op == "%")
        return lValue % rValue;
    else if(this->op == "&")
        return lValue & rValue;
    else if(this->op == "|")
        return lValue | rValue;
    else if(this->op == "^")
        return lValue ^ rValue;
    else if(this->op == "&&")
        return lValue && rValue;
    else if(this->op == "||")
        return lValue || rValue;
    else if(this->op == "==")
        return lValue == rValue;
    else if(this->op == "!=")
        return lValue != rValue;
    else if(this->op == "<")
        return lValue < rValue;
    else if(this->op == ">")
        return lValue > rValue;
    else if(this->op == "<=")
        return lValue <= rValue;
    else if(this->op == ">=")
        return lValue >= rValue;
    else if(this->op == "<<")
        return lValue << rValue;
    else if(this->op == ">>")
        return lValue >> rValue;
    else if(this->op == "?")
        return !lValue.isNil() ?
            lValue : rValue;
    else if(this->op == "::") {
        if(lValue.isRegex() && rValue.isString())
            return DynamicObject(std::regex_match(
                rValue.getString(),
                lValue.getRegex()->getRegex()
            ));
        else if(lValue.isString() && rValue.isRegex())
            return DynamicObject(std::regex_match(
                lValue.getString(),
                rValue.getRegex()->getRegex()
            ));
    }
    else if(this->op == "!:") {
        if(lValue.isRegex() && rValue.isString())
            return DynamicObject(!std::regex_match(
                rValue.getString(),
                lValue.getRegex()->getRegex()
            ));
        else if(lValue.isString() && rValue.isRegex())
            return DynamicObject(!std::regex_match(
                lValue.getString(),
                rValue.getRegex()->getRegex()
            ));
    }
    else if(this->op == ".+") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorAdd(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorAdd(lValue);
    }
    else if(this->op == ".-") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorSub(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorSub(lValue);
    }
    else if(this->op == "./") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorDiv(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorDiv(lValue);
    }
    else if(this->op == ".*") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorMul(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorMul(lValue);
    }
    else if(this->op == ".%") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorRem(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorRem(lValue);
    }
    else if(this->op == ".|") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorBitwiseOr(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorBitwiseOr(lValue);
    }
    else if(this->op == ".&") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorBitwiseAnd(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorBitwiseAnd(lValue);
    }
    else if(this->op == ".^") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorBitwiseXor(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorBitwiseXor(lValue);
    }
    else if(this->op == ".<<") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorShiftLeft(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorShiftLeft(lValue);
    }
    else if(this->op == ".>>") {
        if(lValue.isNumber() && rValue.isArray())
            return lValue.vectorShiftRight(rValue);
        else if(lValue.isArray() && rValue.isNumber())
            return rValue.vectorShiftRight(lValue);
    }

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for type '" + lValue.objectType() +
            "' and '" + rValue.objectType() + "'."
    );

    return {};
}
