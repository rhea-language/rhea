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

#include <N8.hpp>
#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/expression/FunctionDeclarationExpression.hpp>
#include <n8/core/SymbolTable.hpp>
#include <n8/parser/Token.hpp>

FunctionDeclarationExpression& FunctionDeclarationExpression::operator=(
    FunctionDeclarationExpression&& other
) noexcept {
    if(this != &other) {
        this->address = std::move(other.address);
        this->parameters = std::move(other.parameters);
        this->body = std::move(other.body);
    }

    return *this;
}

DynamicObject FunctionDeclarationExpression::visit(
    SymbolTable& symbols
    __attribute__((unused))
) {
    return DynamicObject(
        std::make_shared<FunctionDeclarationExpression>(
            std::move(*this)
        )
    );
}

Token FunctionDeclarationExpression::getFunctionImage() const {
    return *this->address;
}

DynamicObject FunctionDeclarationExpression::call(
    const SymbolTable& symbols,
    const std::vector<DynamicObject>& args
) {
    if(args.size() != this->parameters.size())
        throw ASTNodeException(
            std::move(this->address),
            "Argument count mismatch"
        );

    SymbolTable localSymbols(const_cast<SymbolTable&>(symbols));
    parsync(size_t i = 0; i < args.size(); ++i)
        localSymbols.setSymbol(
            this->parameters[i],
            std::move(args).at(i)
        );

    return this->body->visit(localSymbols);
}
