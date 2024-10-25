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
#include <zhivo/ast/expression/ArrayAccessExpression.hpp>
#include <zhivo/ast/expression/BinaryExpression.hpp>
#include <zhivo/ast/expression/VariableAccessExpression.hpp>
#include <zhivo/parser/Token.hpp>
#include <zhivo/util/VectorMath.hpp>

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
            #ifdef _MSC_VER
            #   pragma warning(disable : 5272)
            #endif
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
        std::unique_ptr<DynamicObject> rValuePtr = std::make_unique<DynamicObject>(rValue);

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
        symbols.setSymbol(varAccess->getName().getImage(), value);        

        return value;
    }

    DynamicObject lValue = this->left->visit(symbols);
    DynamicObject rValue = this->right->visit(symbols);

    if(lValue.isNumber() && rValue.isNumber())
        return this->applyNumOp(lValue, rValue);
    else if(lValue.isArray() && rValue.isArray())
        return this->applyArrayOp(lValue, rValue);
    else if((lValue.isString() || rValue.isString()) &&
        !(this->op == "::" || this->op == "!:"))
        return this->applyStringOp(lValue, rValue);
    else if(lValue.isBool() || rValue.isBool())
        return this->applyBoolOp(lValue, rValue);
    else if(lValue.isRegex() || rValue.isRegex())
        return this->applyRegexOp(lValue, rValue);

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for these types."
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
        return DynamicObject(std::fabs(lValue.getNumber() - rValue.getNumber()) <
            std::numeric_limits<double>::epsilon());
    else if(this->op == "!=")
        return DynamicObject(std::fabs(lValue.getNumber() - rValue.getNumber()) >=
            std::numeric_limits<double>::epsilon());
    else if(this->op == "<<")
        return DynamicObject((float) ((unsigned long) lValue.getNumber() << (unsigned long) rValue.getNumber()));
    else if(this->op == ">>")
        return DynamicObject((float) ((unsigned long) lValue.getNumber() >> (unsigned long) rValue.getNumber()));

    throw ASTNodeException(
        std::move(this->address),
        "Unknown operator: " + this->address->getImage()
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
    else if(this->op == "*") {
        unsigned long count = 0;
        std::string output = "";
        std::string str;

        if(lValue.isNumber() && rValue.isString()) {
            count = (unsigned long) lValue.getNumber();
            str = rValue.getString();
        }
        else if(lValue.isString() && rValue.isNumber()) {
            count = (unsigned long) rValue.getNumber();
            str = lValue.getString();
        }

        for(unsigned long i = 0; i < count; i++)
            output += str;
        return DynamicObject(output);
    }
    else if(this->op == "==")
        return DynamicObject(lValue.getString() == rValue.getString());
    else if(this->op == "!=")
        return DynamicObject(lValue.getString() != rValue.getString());

    throw ASTNodeException(
        std::move(this->address),
        "Unknown operator for string: " + this->address->getImage()
    );
}

DynamicObject BinaryExpression::applyBoolOp(DynamicObject& lValue, DynamicObject& rValue) {
    if(this->op == "||")
        return DynamicObject(lValue.booleanEquivalent() || rValue.booleanEquivalent());
    else if(this->op == "&&")
        return DynamicObject(lValue.booleanEquivalent() && rValue.booleanEquivalent());
    else if(this->op == "==")
        return DynamicObject(lValue == rValue);
    else if(this->op == "!=")
        return DynamicObject(!(lValue == rValue));

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for boolean: " + this->address->getImage()
    );
}

DynamicObject BinaryExpression::applyRegexOp(DynamicObject& lValue, DynamicObject& rValue) {
    if(this->op == "::") {
        if(lValue.isRegex() && rValue.isString()) {
            std::smatch matches;
            auto recipientString = rValue.toString();

            if(std::regex_search(
                recipientString,
                matches,
                lValue.getRegex()->getRegex()
            )) return DynamicObject(true);
            else return DynamicObject(false);
        }
        else if(lValue.isString() && rValue.isRegex()) {
            std::smatch matches;
            auto recipientString = lValue.toString();

            if(std::regex_search(
                recipientString,
                matches,
                rValue.getRegex()->getRegex()
            )) return DynamicObject(true);
            else return DynamicObject(false);
        }
    }
    else if(this->op == "!:") {
        if(lValue.isRegex() && rValue.isString()) {
            std::smatch matches;
            auto recipientString = rValue.toString();

            if(!std::regex_search(
                recipientString,
                matches,
                lValue.getRegex()->getRegex()
            )) return DynamicObject(true);
            else return DynamicObject(false);
        }
        else if(lValue.isString() && rValue.isRegex()) {
            std::smatch matches;
            auto recipientString = lValue.toString();

            if(!std::regex_search(
                recipientString,
                matches,
                rValue.getRegex()->getRegex()
            )) return DynamicObject(true);
            else return DynamicObject(false);
        }
    }

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for regular expression matching."
    );
}

DynamicObject BinaryExpression::applyArrayOp(DynamicObject& lValue, DynamicObject& rValue) {
    for(auto& object : *lValue.getArray())
        if(!object.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Unsupported binary operation for array that contains non-numbers."
            );

    for(auto& object : *rValue.getArray())
        if(!object.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Unsupported binary operation for array that contains non-numbers."
            );

    if(this->op == "+")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::add(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "-")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::sub(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "*")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::mul(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "/")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::div(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "%")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::rem(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "|")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::bitwiseOr(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "&")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::bitwiseAnd(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "<<")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::shiftLeft(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == ">>")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::shiftRight(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );
    else if(this->op == "^")
        return ZhivoUtil::vector2Object(
            ZhivoUtil::VectorMath::bitwiseXor(
                ZhivoUtil::object2Vector(lValue.getArray()),
                ZhivoUtil::object2Vector(rValue.getArray())
            )
        );

    throw ASTNodeException(
        std::move(this->address),
        "Unsupported operation for array objects."
    );
}
