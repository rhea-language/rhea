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

#ifndef ZHIVO_SYMBOL_TABLE_HPP
#define ZHIVO_SYMBOL_TABLE_HPP

#include <ast/ASTNode.hpp>
#include <core/DynamicObject.hpp>

#include <memory>
#include <unordered_map>

class SymbolTable {
private:
    SymbolTable* parent;
    std::unordered_map<std::string, DynamicObject> table;

public:
    explicit SymbolTable(SymbolTable* _parent = nullptr) :
        parent(_parent),
        table({}) {}

    explicit SymbolTable(const SymbolTable& other) :
        parent(other.parent),
        table(other.table) {}

    SymbolTable& operator=(const SymbolTable& other);

    DynamicObject getSymbol(const std::string& name);
    void setSymbol(const std::string& name, DynamicObject value);
    bool hasSymbol(const std::string& name);
};

#endif
