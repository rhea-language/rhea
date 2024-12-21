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
#include <n8/core/Runtime.hpp>
#include <n8/core/SymbolTable.hpp>

SymbolTable& SymbolTable::operator=(SymbolTable&& other) noexcept {
    if(this != &other) {
        parent = std::move(other.parent);
        id = std::move(other.id);
        table = std::move(other.table);
        tasks = std::move(other.tasks);
    }

    return *this;
}

SymbolTable& SymbolTable::operator=(const SymbolTable& other) {
    if(this != &other) {
        this->parent = other.parent;
        this->table = other.table;
        this->id = N8Util::generateUuid();

        this->tasks.clear();
    }

    return *this;
}

DynamicObject SymbolTable::getSymbol(
    std::shared_ptr<Token> reference,
    const std::string& name
) {
    if(this->parent && this->parent->hasSymbol(name))
        return this->parent->getSymbol(
            std::move(reference),
            name
        );

    if(this->hasSymbol(name))
        return this->table[name];

    throw ASTNodeException(
        std::move(reference),
        "Cannot resolve symbol: " + name
    );
}

void SymbolTable::setSymbol(
    std::shared_ptr<Token> reference,
    DynamicObject value
) {
    std::string name = reference->getImage();
    if(this->table.find(name) != this->table.end()) {
        DynamicObject& current = this->table[name];
        if(current.hasLock())
            return;

        current = std::move(value);
    }
    else if(this->parent && this->parent->hasSymbol(name))
        this->parent->setSymbol(std::move(reference), std::move(value));
    else this->table[name] = std::move(value);
}

void SymbolTable::removeSymbol(const std::string& name) {
    if(this->parent && this->parent->hasSymbol(name)) {
        this->parent->removeSymbol(name);
        return;
    }

    if(this->hasSymbol(name) && !this->table[name].hasLock())
        this->table.erase(name);
}

bool SymbolTable::hasSymbol(const std::string& name) {
    return (this->parent && this->parent->hasSymbol(name)) ||
        this->table.find(name) != this->table.end();
}

void SymbolTable::addParallelism(std::future<void> par) {
    this->tasks.push_back(std::move(par));
}

void SymbolTable::waitForTasks() {
    if(!this->tasks.empty()) {
        for(auto& task : this->tasks)
            if(task.valid())
                task.get();

        this->tasks.clear();
    }

    if(this->parent)
        this->parent->waitForTasks();
}

void SymbolTable::lock(std::string name, SymbolTable& requestOrigin) {
    if(!this->hasSymbol(name))
        return;

    if(this->table.find(name) != this->table.end()) {
        this->table[name].own(requestOrigin.id);
        this->table[name].lock();
    }
    else if(this->parent)
        this->parent->lock(name, requestOrigin);
}

void SymbolTable::unlock(std::string name, SymbolTable& requestOrigin) {
    if(!this->hasSymbol(name))
        return;

    if(this->table.find(name) != this->table.end() &&
        this->table[name].ownerId() == requestOrigin.id)
        this->table[name].unlock();
    else if(this->parent)
        this->parent->unlock(name, requestOrigin);
}
