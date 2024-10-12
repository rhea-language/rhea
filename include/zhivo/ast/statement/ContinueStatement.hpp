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

#ifndef ZHIVO_AST_STMT_CONTINUE_HPP
#define ZHIVO_AST_STMT_CONTINUE_HPP

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/ast/TerminativeSignal.hpp>
#include <zhivo/core/DynamicObject.hpp>
#include <zhivo/core/SymbolTable.hpp>
#include <zhivo/parser/Token.hpp>

class ContinueStatement : public ASTNode {
public:
    explicit ContinueStatement(std::unique_ptr<Token> _address) {
        this->address = std::move(_address);
    }

    [[nodiscard, noreturn]]
    DynamicObject visit(SymbolTable& symbols) override;
};

#endif
