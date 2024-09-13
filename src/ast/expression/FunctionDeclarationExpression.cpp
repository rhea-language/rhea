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

#include <ast/expression/FunctionDeclarationExpression.hpp>
#include <core/SymbolTable.hpp>

FunctionDeclarationExpression& FunctionDeclarationExpression::operator=(FunctionDeclarationExpression&& other) noexcept {
    if(this != &other) {
        this->address = std::move(other.address);
        this->parameters = std::move(other.parameters);
        this->body = std::move(other.body);
    }

    return *this;
}

DynamicObject FunctionDeclarationExpression::visit(SymbolTable& symbols __attribute__((unused))) {
    return DynamicObject(std::make_shared<FunctionDeclarationExpression>(std::move(*this)));
}

Token FunctionDeclarationExpression::getFunctionImage() const {
    return *this->address;
}

DynamicObject FunctionDeclarationExpression::call(SymbolTable& symbols, const std::vector<DynamicObject>& args) {
    if(args.size() != this->parameters.size())
        throw std::runtime_error("Argument count mismatch");

    SymbolTable localSymbols(&symbols);
    for(size_t i = 0; i < args.size(); ++i) {
        auto obj = std::move(&std::move(args).at(i));
        localSymbols.setSymbol(this->parameters[i]->getImage(), obj);
    }

    return std::move(this->body->visit(localSymbols));
}
