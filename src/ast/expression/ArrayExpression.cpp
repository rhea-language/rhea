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

#include <ast/expression/ArrayExpression.hpp>
#include <memory>

DynamicObject ArrayExpression::visit(SymbolTable& symbols) {
    std::shared_ptr<std::vector<DynamicObject>> objects =
        std::make_shared<std::vector<DynamicObject>>();

    for(size_t i = 0; i < this->elements.size(); i++)
        objects->push_back(std::move(this->elements.at(i)->visit(symbols)));

    return DynamicObject(objects);
}
