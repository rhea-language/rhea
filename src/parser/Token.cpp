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

#include <parser/Token.hpp>

bool Token::operator==(const Token& other) {
    return this->image == other.image &&
        this->fileName == other.fileName &&
        this->line == other.line &&
        this->column == other.column &&
        this->type == other.type;
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

TokenType Token::getType() const {
    return this->type;
}

std::string Token::toString() const {
    return this->image + " [line " +
        std::to_string(this->line) + ", column " +
        std::to_string(this->column) + "] (" +
        this->fileName +
        ")";
}
