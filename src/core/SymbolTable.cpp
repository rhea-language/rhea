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
#include <core/SymbolTable.hpp>

SymbolTable& SymbolTable::operator=(const SymbolTable& other) {
    if(this != &other) {
        this->parent = std::move(other.parent);
        this->table = std::move(other.table);
    }

    return *this;
}

DynamicObject SymbolTable::getSymbol(
    std::unique_ptr<Token> reference,
    const std::string& name
) {
    if(this->hasSymbol(name))
        return std::move(this->table[name]);
    else if(this->parent)
        return this->parent->getSymbol(
            std::move(reference),
            name
        );

    throw ASTNodeException(
        std::move(reference),
        "Cannot resolve symbol: " + name
    );
}

void SymbolTable::setSymbol(const std::string& name, DynamicObject value) {
    if(this->parent && this->parent->hasSymbol(name))
        this->parent->setSymbol(name, std::move(value));

    this->table[name] = std::move(value);
}

bool SymbolTable::hasSymbol(const std::string& name) {
    return this->table.find(name) != this->table.end() ||
        (this->parent && this->parent->hasSymbol(name));
}
