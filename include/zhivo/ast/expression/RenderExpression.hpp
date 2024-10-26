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

#ifndef ZHIVO_AST_EXPR_RENDER_HPP
#define ZHIVO_AST_EXPR_RENDER_HPP

#include <zhivo/ast/ASTNode.hpp>

class RenderExpression final : public ASTNode {
private:
    std::shared_ptr<ASTNode> expression;
    bool newLine;
    bool errorStream;

public:
    explicit RenderExpression(
        std::shared_ptr<Token> _address,
        bool _newLine,
        bool _errorStream,
        std::shared_ptr<ASTNode> _expression
    ) : expression(std::move(_expression)),
        newLine(_newLine),
        errorStream(_errorStream) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif
