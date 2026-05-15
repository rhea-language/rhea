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

#include <memory>
#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/ast/expression/ArrayAccessExpression.hpp>
#include <rhea/parser/Token.hpp>

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
            "Accessing non-array and non-string object is invalid.");

    DynamicObject idx = this->index->visit(symbols);
    if(origin.isString()) {
        if(!idx.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Accessing string with non-number index is not allowed.");

        double rawIdx = idx.getNumber();
        if(rawIdx < 0)
            throw ASTNodeException(std::move(this->address),
                                   "String index cannot be negative.");

        size_t i = static_cast<size_t>(rawIdx);
        const std::string& str = origin.getString();
        if(i >= str.size())
            throw ASTNodeException(std::move(this->address),
                                   "String index " + std::to_string(i) +
                                       " is out of bounds (size=" +
                                       std::to_string(str.size()) + ").");

        return DynamicObject(std::string(1, str[i]));
    } else if(origin.isArray()) {
        if(!idx.isNumber())
            throw ASTNodeException(
                std::move(this->address),
                "Accessing array with non-number index is not allowed.");

        double rawIdx = idx.getNumber();
        if(rawIdx < 0)
            throw ASTNodeException(std::move(this->address),
                                   "Array index cannot be negative.");

        size_t i = static_cast<size_t>(rawIdx);
        auto arr = origin.getArray();
        if(i >= arr->size())
            throw ASTNodeException(std::move(this->address),
                                   "Array index " + std::to_string(i) +
                                       " is out of bounds (size=" +
                                       std::to_string(arr->size()) + ").");

        return DynamicObject(std::move(arr->at(i)));
    }

    throw ASTNodeException(std::move(this->address),
                           "Dynamic object is not of array type.");
}
