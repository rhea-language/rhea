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

#ifndef ZHIVO_AST_EPXR_FUNC_DECL_HPP
#define ZHIVO_AST_EXPR_FUNC_DECL_HPP

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/ast/TerminativeSignal.hpp>
#include <zhivo/parser/Token.hpp>

#include <memory>
#include <string>
#include <vector>

class FunctionDeclarationExpression : public ASTNode {
private:
    std::vector<std::unique_ptr<Token>> parameters;
    std::unique_ptr<ASTNode> body;

public:
    explicit FunctionDeclarationExpression(
        std::unique_ptr<Token> _address,
        std::vector<std::unique_ptr<Token>> _parameters,
        std::unique_ptr<ASTNode> _body
    ) : parameters(std::move(_parameters)),
        body(std::move(_body)) {
        this->address = std::move(_address);
    }

    explicit FunctionDeclarationExpression(
        FunctionDeclarationExpression&& other
    ) noexcept :
        parameters(std::move(other.parameters)),
        body(std::move(other.body)) {
        this->address = std::move(other.address);
    }

    FunctionDeclarationExpression(const FunctionDeclarationExpression&) = delete;
    FunctionDeclarationExpression& operator=(const FunctionDeclarationExpression&) = delete;
    FunctionDeclarationExpression& operator=(FunctionDeclarationExpression&& other) noexcept;

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
    DynamicObject call(
        const SymbolTable& symbols,
        const std::vector<DynamicObject>& args
    );

    Token getFunctionImage() const;
};

#endif
