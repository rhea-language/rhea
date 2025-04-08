/*
 * Copyright (c) 2025 - Nathanne Isip
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

#ifndef RHEA_AST_STMT_ENUM_HPP
#define RHEA_AST_STMT_ENUM_HPP

#include <rhea/ast/ASTNode.hpp>
#include <rhea/parser/Token.hpp>

#include <map>
#include <memory>

class EnumStatement final : public ASTNode {
private:
    std::shared_ptr<Token> name;
    std::map<std::shared_ptr<Token>, std::shared_ptr<ASTNode>> list;

public:
    explicit EnumStatement(
        std::shared_ptr<Token> _address,
        std::shared_ptr<Token> _name,
        std::map<std::shared_ptr<Token>, std::shared_ptr<ASTNode>> _list
    ) : name(std::move(_name)),
        list(std::move(_list)) {
        this->address = std::move(_address);
    }

    [[nodiscard]]
    DynamicObject visit(SymbolTable& symbols) override;
};

#endif
