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

#include <Rhea.hpp>
#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/core/Runtime.hpp>
#include <rhea/core/SymbolTable.hpp>

SymbolTable& SymbolTable::operator=(SymbolTable&& other) noexcept {
    if(this != &other) {
        std::lock_guard<std::recursive_mutex> lock1(this->mtx);
        std::lock_guard<std::recursive_mutex> lock2(other.mtx);

        parent = std::move(other.parent);
        id = std::move(other.id);
        table = std::move(other.table);
        tasks = std::move(other.tasks);
    }

    return *this;
}

SymbolTable& SymbolTable::operator=(const SymbolTable& other) {
    if(this != &other) {
        std::lock_guard<std::recursive_mutex> lock1(this->mtx);
        std::lock_guard<std::recursive_mutex> lock2(other.mtx);

        this->parent = other.parent;
        this->table = other.table;
        this->id = RheaUtil::uniqueKey();

        this->tasks.clear();
    }

    return *this;
}

DynamicObject SymbolTable::getSymbol(std::shared_ptr<Token> reference,
                                     const std::string& name) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);

    if(this->parent && this->parent->hasSymbol(name))
        return this->parent->getSymbol(std::move(reference), name);

    if(this->table.count(name) == 1) return this->table[name];

    throw ASTNodeException(std::move(reference),
                           "Cannot resolve symbol: " + name);
}

void SymbolTable::setSymbol(std::shared_ptr<Token> reference,
                            DynamicObject value) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);

    std::string name = reference->getImage();
    if(this->table.count(name) == 1) {
        DynamicObject& current = this->table[name];
        if(current.hasLock()) return;

        current = std::move(value);
    } else if(this->parent && this->parent->hasSymbol(name))
        this->parent->setSymbol(std::move(reference), std::move(value));
    else
        this->table[name] = std::move(value);
}

void SymbolTable::removeSymbol(std::string name) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);

    if(this->parent && this->parent->hasSymbol(name)) {
        this->parent->removeSymbol(name);
        return;
    }

    if(this->table.count(name) == 1 && !this->table[name].hasLock())
        this->table.erase(name);
}

void SymbolTable::removeSymbol(std::shared_ptr<Token> name) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);
    std::string symbol = name->getImage();

    if(this->parent && this->parent->hasSymbol(symbol)) {
        this->parent->removeSymbol(name);
        return;
    }

    if(this->table.count(symbol) == 1 && !this->table[symbol].hasLock()) {
        this->table.erase(symbol);
        return;
    }

    throw ASTNodeException(std::move(name), "Cannot remove symbol: " + symbol);
}

bool SymbolTable::hasSymbol(const std::string& name) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);

    return (this->parent && this->parent->hasSymbol(name)) ||
           this->table.count(name) == 1;
}

void SymbolTable::addParallelism(std::future<void> par) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);
    this->tasks.push_back(std::move(par));
}

void SymbolTable::waitForTasks() {
    std::vector<std::future<void>> currentTasks;
    {
        std::lock_guard<std::recursive_mutex> lock(this->mtx);
        currentTasks = std::move(this->tasks);
        this->tasks.clear();
    }

    if(!currentTasks.empty()) {
        parsync(size_t i = 0; i < currentTasks.size(); ++i) {
            auto& task = currentTasks[i];
            if(task.valid()) task.get();
        }
    }

    if(this->parent) this->parent->waitForTasks();
}

void SymbolTable::lock(std::string name, SymbolTable& requestOrigin) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);

    if(this->table.count(name) == 1) {
        DynamicObject& current = this->table[name];
        if(current.hasLock()) return;

        current.own(requestOrigin.id);
        current.lock();
    } else if(this->parent)
        this->parent->lock(name, requestOrigin);
}

void SymbolTable::unlock(std::string name, SymbolTable& requestOrigin) {
    std::lock_guard<std::recursive_mutex> lock(this->mtx);

    if(this->table.count(name) == 1 &&
       this->table[name].ownerId() == requestOrigin.id)
        this->table[name].unlock();
    else if(this->parent)
        this->parent->unlock(name, requestOrigin);
}
