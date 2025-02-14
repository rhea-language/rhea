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

#ifndef N8_PARSER_TOKENIZER_HPP
#define N8_PARSER_TOKENIZER_HPP

#include <n8/parser/LexicalAnalysisException.hpp>
#include <n8/parser/OperatorsAndKeys.hpp>
#include <n8/parser/Token.hpp>
#include <n8/parser/TokenCategory.hpp>

#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class Tokenizer final {
private:
    std::string source;
    std::string fileName;
    std::vector<Token> tokens;

    int length = 0;
    int index = 0;

    bool isAtEnd() const;

    static bool isDigit(char ch);
    static bool isBinaryDigit(char ch);
    static bool isTrinaryDigit(char ch);
    static bool isOctalDecimalDigit(char ch);
    static bool isHexadecimalDigit(char ch);

    static bool isAlphabet(char ch);
    static bool isOperator(char ch);
    static bool isWhitespace(char ch);
    static bool isKeyword(const std::string& image);

public:
    Tokenizer(std::string _source, std::string _fileName) :
        source(std::move(_source)),
        fileName(std::move(_fileName)),
        tokens({}),
        length((int) this->source.length()),
        index(0) {}

    static std::shared_ptr<Tokenizer> loadFile(const std::string& filePath);
    static bool isValidIdentifier(std::string str);

    void scan();
    const std::vector<Token>& getTokens() const;
};

#endif
