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

#ifndef ZHIVO_AST_EXPR_BLOCK_HPP
#define ZHIVO_AST_EXPR_BLOCK_HPP

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/ast/TerminativeSignal.hpp>

#include <memory>
#include <vector>

class BlockExpression : public ASTNode {
private:
    std::vector<std::unique_ptr<ASTNode>> statements;

public:
    explicit BlockExpression(
        std::unique_ptr<Token> _address,
        std::vector<std::unique_ptr<ASTNode>> _statements
    ) : statements(std::move(_statements)) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif