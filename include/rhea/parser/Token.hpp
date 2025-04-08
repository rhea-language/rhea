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

#ifndef RHEA_PARSER_TOKEN_HPP
#define RHEA_PARSER_TOKEN_HPP

#include <rhea/parser/TokenCategory.hpp>
#include <string>

class Token final {
private:
    std::string image;
    std::string fileName;

    int line;
    int column;

    TokenCategory type;

public:
    Token(
        std::string _image,
        std::string _fileName,
        int _line,
        int _column,
        TokenCategory _type
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

    TokenCategory getType() const;
    std::string toString() const;

    void appendToImage(std::string str);
    void modifyImage(std::string str);
};

#endif