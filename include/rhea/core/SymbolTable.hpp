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

#ifndef RHEA_SYMBOL_TABLE_HPP
#define RHEA_SYMBOL_TABLE_HPP

#include <rhea/ast/ASTNode.hpp>
#include <rhea/core/DynamicObject.hpp>
#include <rhea/util/RandomUtil.hpp>

#include <future>
#include <memory>
#include <unordered_map>
#include <vector>

class SymbolTable final {
private:
    std::shared_ptr<SymbolTable> parent;
    std::string id;
    std::unordered_map<std::string, DynamicObject> table;
    std::vector<std::future<void>> tasks;

public:
    explicit SymbolTable(std::string _id, std::shared_ptr<SymbolTable> _parent = nullptr) :
        parent(std::move(_parent)),
        id(std::move(_id)),
        table({}),
        tasks() {}

    explicit SymbolTable(std::shared_ptr<SymbolTable> _parent = nullptr) :
        parent(std::move(_parent)),
        id(RheaUtil::uniqueKey()),
        table({}),
        tasks() {}

    SymbolTable(const SymbolTable& other) :
        parent(other.parent),
        id(other.id),
        table(other.table),
        tasks() {}

    SymbolTable& operator=(SymbolTable&& other) noexcept;
    SymbolTable& operator=(const SymbolTable& other);

    DynamicObject getSymbol(
        std::shared_ptr<Token> reference,
        const std::string& name
    );

    void setSymbol(
        std::shared_ptr<Token> reference,
        DynamicObject value
    );

    void removeSymbol(std::string name);
    void removeSymbol(std::shared_ptr<Token> name);
    bool hasSymbol(const std::string& name);

    void addParallelism(std::future<void> par);
    void waitForTasks();

    void lock(std::string name, SymbolTable& requestOrigin);
    void unlock(std::string name, SymbolTable& requestOrigin);
};

#endif
