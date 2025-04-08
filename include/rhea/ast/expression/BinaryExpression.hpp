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

#ifndef RHEA_AST_EXPR_BINARY_HPP
#define RHEA_AST_EXPR_BINARY_HPP

#include <rhea/ast/ASTNode.hpp>
#include <rhea/core/DynamicObject.hpp>
#include <rhea/core/SymbolTable.hpp>

#include <cmath>

class BinaryExpression final : public ASTNode {
private:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    std::string op;

public:
    explicit BinaryExpression(
        std::shared_ptr<Token> _address,
        std::shared_ptr<ASTNode> _left,
        std::string _op,
        std::shared_ptr<ASTNode> _right
    ) : left(std::move(_left)),
        right(std::move(_right)),
        op(_op) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif