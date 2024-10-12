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

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/core/DynamicObject.hpp>

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

class SymbolTable {
private:
    SymbolTable* parent;
    std::unordered_map<std::string, DynamicObject> table;
    std::vector<std::thread> threads;
    mutable std::mutex mtx;

public:
    explicit SymbolTable(SymbolTable* _parent = nullptr) :
        parent(_parent),
        table({}),
        threads(),
        mtx() {}

    explicit SymbolTable(const SymbolTable& other) :
        parent(std::move(other.parent)),
        table(std::move(other.table)),
        threads(),
        mtx() {}

    SymbolTable& operator=(const SymbolTable& other);

    DynamicObject getSymbol(
        std::unique_ptr<Token> reference,
        const std::string& name
    );

    void setSymbol(const std::string& name, DynamicObject value);
    bool hasSymbol(const std::string& name);

    void addParallelism(std::thread par);
    void waitForThreads();
    void detachParallelNodes();
};

#endif
