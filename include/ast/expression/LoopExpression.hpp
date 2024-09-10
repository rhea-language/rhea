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

#ifndef ZHIVO_AST_EXPR_LOOP_HPP
#define ZHIVO_AST_EXPR_LOOP_HPP

#include <ast/ASTNode.hpp>
#include <ast/TerminativeSignal.hpp>

#include <memory>
#include <vector>

class LoopExpression : public ASTNode {
private:
    std::unique_ptr<ASTNode> initial;
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> postexpr;
    std::unique_ptr<ASTNode> body;

public:
    explicit LoopExpression(
        std::unique_ptr<Token> _address,
        std::unique_ptr<ASTNode> _initial,
        std::unique_ptr<ASTNode> _condition,
        std::unique_ptr<ASTNode> _postexpr,
        std::unique_ptr<ASTNode> _body
    ) : initial(std::move(_initial)),
        condition(std::move(_condition)),
        postexpr(std::move(_postexpr)),
        body(std::move(_body)) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif
