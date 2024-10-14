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

#ifndef ZHIVO_AST_EXPR_BINARY_HPP
#define ZHIVO_AST_EXPR_BINARY_HPP

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/core/DynamicObject.hpp>
#include <zhivo/core/SymbolTable.hpp>

#include <cmath>

class BinaryExpression final : public ASTNode {
private:
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    std::string op;

    DynamicObject applyStringOp(DynamicObject& lValue, DynamicObject& rValue);
    DynamicObject applyNumOp(DynamicObject& lValue, DynamicObject& rValue);
    DynamicObject applyBoolOp(DynamicObject& lValue, DynamicObject& rValue);
    DynamicObject applyRegexOp(DynamicObject& lValue, DynamicObject& rValue);
    DynamicObject applyArrayOp(DynamicObject& lValue, DynamicObject& rValue);

public:
    explicit BinaryExpression(
        std::unique_ptr<Token> _address,
        std::unique_ptr<ASTNode> _left,
        std::string _op,
        std::unique_ptr<ASTNode> _right
    ) : left(std::move(_left)),
        right(std::move(_right)),
        op(_op) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif