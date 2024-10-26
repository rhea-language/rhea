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

#ifndef ZHIVO_AST_EXPR_CATCH_HANDLE_HPP
#define ZHIVO_AST_EXPR_CATCH_HANDLE_HPP

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/ast/TerminativeSignal.hpp>
#include <zhivo/parser/Token.hpp>

#include <memory>
#include <vector>

class CatchHandleExpression final : public ASTNode {
private:
    std::shared_ptr<ASTNode> catchBlock;
    std::shared_ptr<ASTNode> handleBlock;
    std::shared_ptr<Token> handler;
    std::shared_ptr<ASTNode> finalBlock;

public:
    explicit CatchHandleExpression(
        std::shared_ptr<Token> _address,
        std::shared_ptr<ASTNode> _catchBlock,
        std::shared_ptr<ASTNode> _handleBlock,
        std::shared_ptr<Token> _handler,
        std::shared_ptr<ASTNode> _finalBlock
    ) : catchBlock(std::move(_catchBlock)),
        handleBlock(std::move(_handleBlock)),
        handler(std::move(_handler)),
        finalBlock(std::move(_finalBlock)) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif
