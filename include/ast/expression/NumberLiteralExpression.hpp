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

#ifndef ZHIVO_AST_EXPR_NUMERIC_LITERAL_HPP
#define ZHIVO_AST_EXPR_NUMERIC_LITERAL_HPP

#include <ast/ASTNode.hpp>
#include <core/DynamicObject.hpp>
#include <core/SymbolTable.hpp>

class NumberLiteralExpression : public ASTNode {
private:
    double value;

public:
    explicit NumberLiteralExpression(
        std::unique_ptr<Token> _address,
        double _value
    ) : value(_value) {
        this->address = std::move(_address);
    }

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols);
};

#endif
