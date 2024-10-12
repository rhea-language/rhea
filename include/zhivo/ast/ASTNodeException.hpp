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

#ifndef ZHIVO_AST_NODE_EXCEPTION_HPP
#define ZHIVO_AST_NODE_EXCEPTION_HPP

#include <zhivo/parser/Token.hpp>

#include <memory>
#include <stdexcept>

class ASTNodeException : public std::runtime_error {
private:
    std::unique_ptr<Token> address;

public:
    explicit ASTNodeException(
        std::unique_ptr<Token> _address,
        const std::string& message) :
        std::runtime_error(message),
        address(std::move(_address)) {}

    inline const Token* getAddress() const {
        return this->address.get();
    }
};

#endif
