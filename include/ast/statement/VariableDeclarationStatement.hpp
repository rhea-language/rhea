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

#ifndef ZHIVO_AST_STMT_VAR_DECL_HPP
#define ZHIVO_AST_STMT_VAR_DECL_HPP

#include <ast/ASTNode.hpp>
#include <parser/Token.hpp>

#include <map>
#include <memory>

class VariableDeclarationStatement : public ASTNode {
private:
    std::map<Token, std::unique_ptr<ASTNode>> declarations;
    std::string nativePath;

public:
    explicit VariableDeclarationStatement(
        std::unique_ptr<Token> _address,
        std::map<Token, std::unique_ptr<ASTNode>> _declarations,
        std::string _nativePath
    ) : declarations(std::move(_declarations)),
        nativePath(_nativePath) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
    static NativeFunction loadNativeFunction(
        std::string& libName,
        std::string& funcName,
        std::unique_ptr<Token> address
    );
};

#endif
