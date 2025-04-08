/*
 * Copyright (c) 2025 - Nathanne Isip
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

#include <rhea/ast/statement/EnumStatement.hpp>
#include <rhea/core/SymbolTable.hpp>

DynamicObject EnumStatement::visit(
    SymbolTable& symbols
) {
    for(const auto& pair : this->list) {
        Token itemName = *pair.first;
        itemName.modifyImage(
            this->name->getImage() +
            "." +
            itemName.getImage()
        );

        symbols.setSymbol(
            std::make_shared<Token>(itemName),
            pair.second->visit(symbols)
        );
    }

    return {};
}
