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

#include <ast/expression/ArrayAccessExpression.hpp>
#include <memory>

DynamicObject ArrayAccessExpression::visit(SymbolTable& symbols) {
    DynamicObject origin = this->array->visit(symbols);
    if(!origin.isArray() && !origin.isString())
        throw std::runtime_error("Accessing non-array and non-string object is invalid.");

    DynamicObject idx = this->index->visit(symbols);
    if(origin.isString()) {
        if(!idx.isNumber())
            throw std::runtime_error("Accessing string with non-number index is not allowed.");

        return DynamicObject(std::move(
            std::string(1, origin.getString().at((int) idx.getNumber()))
        ));
    }
    else if(origin.isArray()) {
        if(!idx.isNumber())
            throw std::runtime_error("Accessing array with non-number index is not allowed.");

        return DynamicObject(std::move(origin.getArray()->at((int) idx.getNumber())));
    }

    throw std::runtime_error("Dynamic object is not of array type.");
}
