/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef N8_PARSER_TOKENTYPE_HPP
#define N8_PARSER_TOKENTYPE_HPP

#include <cstdint>
#include <string>

class TokenType {
private:
    uint8_t type;

public:
    explicit TokenType(uint8_t _type = 0) : type(_type) { }

    static const TokenType DIGIT;
    static const TokenType STRING;
    static const TokenType REGEX;
    static const TokenType KEYWORD;
    static const TokenType IDENTIFIER;
    static const TokenType OPERATOR;

    uint8_t getValue() const { return type; }

    bool operator==(const TokenType& other) const { return type == other.type; }
    bool operator!=(const TokenType& other) const { return type != other.type; }

    bool operator<(const TokenType& other) const { return type < other.type; }
    bool operator>(const TokenType& other) const { return type > other.type; }
};

inline std::string tokenTypeToString(TokenType type) {
    if(type == TokenType::DIGIT)
        return "digit";
    if(type == TokenType::STRING)
        return "string";
    if(type ==  TokenType::KEYWORD)
        return "keyword";
    if(type == TokenType::IDENTIFIER)
        return "identifier";
    if(type == TokenType::OPERATOR)
        return "operator";
    if(type == TokenType::REGEX)
        return "regular expression";

    return "unknown";
}

#endif
