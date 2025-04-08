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

#ifndef RHEA_AST_STMT_DELETE_HPP
#define RHEA_AST_STMT_DELETE_HPP

#include <rhea/ast/ASTNode.hpp>
#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/core/DynamicObject.hpp>
#include <rhea/core/SymbolTable.hpp>
#include <rhea/parser/Token.hpp>

class DeleteStatement final : public ASTNode {
private:
    std::vector<std::shared_ptr<Token>> variables;

public:
    explicit DeleteStatement(
        std::shared_ptr<Token> _address,
        std::vector<std::shared_ptr<Token>> _variables
    ) : variables(std::move(_variables)) {
        this->address = std::move(_address);
    }

    [[nodiscard]]
    DynamicObject visit(SymbolTable& symbols) override;
};

#endif
