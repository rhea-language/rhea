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

#include <rhea/parser/Token.hpp>

bool Token::operator==(const Token& other) const {
    return this->image == other.image &&
        this->fileName == other.fileName &&
        this->line == other.line &&
        this->column == other.column &&
        this->type == other.type;
}

bool Token::operator<(const Token& other) const {
    return (this->type != other.type) ?
        this->type < other.type :
        this->image < other.image;
}

std::string Token::getImage() const {
    return this->image;
}

std::string Token::getFileName() const {
    return this->fileName;
}

int Token::getLine() const {
    return this->line;
}

int Token::getColumn() const {
    return this->column;
}

TokenCategory Token::getType() const {
    return this->type;
}

std::string Token::toString() const {
    return "\u001b[1;32m" + this->image + "\u001b[0m [line " +
        std::to_string(this->line) + ", column " +
        std::to_string(this->column) + "] (\u001b[4;97m" +
        this->fileName +
        "\u001b[0m)";
}

void Token::appendToImage(std::string str) {
    this->image += str;
}

void Token::modifyImage(std::string str) {
    this->image = str;
}
