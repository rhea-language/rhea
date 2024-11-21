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

#ifndef N8_AST_EXPR_SIZE_HPP
#define N8_AST_EXPR_SIZE_HPP

#include <n8/ast/ASTNode.hpp>
#include <n8/core/DynamicObject.hpp>
#include <n8/core/SymbolTable.hpp>

#include <string>

class SizeExpression final : public ASTNode {
private:
    std::shared_ptr<ASTNode> expression;

public:
    explicit SizeExpression(
        std::shared_ptr<Token> _address,
        std::shared_ptr<ASTNode> _expression
    ) : expression(std::move(_expression)) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif
