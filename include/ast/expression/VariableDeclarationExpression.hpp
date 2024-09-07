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

#ifndef ZHIVO_AST_EXPR_VAR_DECL_HPP
#define ZHIVO_AST_EXPR_VAR_DECL_HPP

#include <ast/ASTNode.hpp>
#include <core/DynamicObject.hpp>
#include <core/SymbolTable.hpp>

#include <memory>
#include <string>

class VariableDeclarationExpression : public ASTNode {
private:
    std::unique_ptr<Token> name;
    std::unique_ptr<ASTNode> initializer;

public:
    explicit VariableDeclarationExpression(
        std::unique_ptr<Token> _name,
        std::unique_ptr<ASTNode> _initializer
    ) : name(std::move(_name)),
        initializer(std::move(_initializer)) {
        this->address = std::move(_name);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif
