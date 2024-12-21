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

#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/expression/ArrayAccessExpression.hpp>
#include <n8/parser/Token.hpp>

#include <memory>

ASTNode* ArrayAccessExpression::getArrayExpression() const {
    return std::move(this->array.get());
}

ASTNode* ArrayAccessExpression::getIndexExpression() const {
    return std::move(this->index.get());
}

DynamicObject ArrayAccessExpression::visit(SymbolTable& symbols) {
    DynamicObject origin = this->array->visit(symbols);
    if(!origin.isArray() && !origin.isString())
        throw ASTNodeException(
            std::move(this->address),
            "Accessing non-array and non-string object is invalid."
        );

    DynamicObject idx = this->index->visit(symbols);
    if(origin.isString()) {
        if(!idx.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Accessing string with non-number index is not allowed."
            );

        return DynamicObject(
            std::string(1, origin.getString().at((size_t) idx.getNumber()))
        );
    }
    else if(origin.isArray()) {
        if(!idx.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Accessing array with non-number index is not allowed."
            );

        return DynamicObject(std::move(origin.getArray()->at((size_t) idx.getNumber())));
    }

    throw ASTNodeException(
        std::move(this->address),
        "Dynamic object is not of array type."
    );
}
