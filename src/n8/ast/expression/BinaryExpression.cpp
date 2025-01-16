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

#include <N8.hpp>
#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/expression/ArrayAccessExpression.hpp>
#include <n8/ast/expression/BinaryExpression.hpp>
#include <n8/ast/expression/VariableAccessExpression.hpp>
#include <n8/parser/Token.hpp>
#include <n8/util/VectorMath.hpp>

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

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for type '" + lValue.objectType() +
            "' and '" + rValue.objectType() + "'."
    );

    return {};
}
