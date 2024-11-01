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

#ifndef N8_PARSER_TOKEN_HPP
#define N8_PARSER_TOKEN_HPP

#include <n8/parser/TokenType.hpp>
#include <string>

class Token final {
private:
    std::string image;
    std::string fileName;

    int line;
    int column;

    TokenType type;

public:
    Token(
        std::string _image,
        std::string _fileName,
        int _line,
        int _column,
        TokenType _type
    ) : image(std::move(_image)),
        fileName(std::move(_fileName)),
        line(_line),
        column(_column),
        type(_type) {}

    bool operator==(const Token& other) const;
    bool operator<(const Token& other) const;

    std::string getImage() const;
    std::string getFileName() const;

    int getLine() const;
    int getColumn() const;

    TokenType getType() const;
    std::string toString() const;

    void appendToImage(std::string str);
};

#endif