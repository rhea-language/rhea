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

#ifndef RHEA_PARSER_EXCEPTION_HPP
#define RHEA_PARSER_EXCEPTION_HPP

#include <rhea/parser/Token.hpp>

#include <memory>
#include <stdexcept>

class ParserException final : public std::runtime_error {
private:
    std::shared_ptr<Token> address;

public:
    explicit ParserException(
        std::shared_ptr<Token> _address,
        const std::string& message) :
        std::runtime_error(message),
        address(std::move(_address)) {}

    inline const Token* getAddress() const {
        return this->address.get();
    }
};

#endif
