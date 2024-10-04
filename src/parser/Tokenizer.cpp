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

#include <parser/Tokenizer.hpp>
#include <util/StringUnescape.hpp>

std::unique_ptr<Tokenizer> Tokenizer::loadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if(!file.is_open())
        throw std::runtime_error("File not found: " + filePath);

    std::ostringstream content;
    content << file.rdbuf();

    return std::make_unique<Tokenizer>(content.str(), filePath);
}

void Tokenizer::scan() {
    if(this->source.empty())
        return;

    int line = 1, column = 0;
    while(!this->isAtEnd()) {
        char currentChar = this->source[this->index++];
        column++;

        if(this->isWhitespace(currentChar)) {
            if(currentChar == '\n') {
                line++;
                column = 0;
            }
        }
        else if(this->isOperator(currentChar)) {
            if(currentChar == '#') {
                while(!this->isAtEnd() && this->source[this->index] != '\n') {
                    this->index++;
                    column++;
                }

                column = 0;
            }
            else if(currentChar == '"') {
                std::string str;
                int startColumn = column;

                while(!this->isAtEnd() && this->source[this->index] != '"') {
                    char ch = this->source[this->index++];
                    column++;

                    if(ch == '\n')
                        throw LexicalAnalysisException("Found new line inside string literal. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");
                    else if(ch == '\\') {
                        str += ch;
                        column++;

                        if(isAtEnd())
                            throw LexicalAnalysisException("Expecting escape character, encountered end-of-file. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");

                        char escape = this->source[this->index++];
                        str += escape;
                        column++;
                    }
                    else str += ch;
                }

                this->index++;
                column++;

                str = ZhivoUtil::replaceEscapeSequences(std::move(str));
                this->tokens.emplace_back(
                    str,
                    fileName,
                    line,
                    startColumn,
                    TokenType::STRING
                );
            }
            else if(currentChar == '`') {
                std::string str;
                int startColumn = column;

                while(!this->isAtEnd() && this->source[this->index] != '`') {
                    char ch = this->source[this->index++];

                    if(ch == '\n')
                        throw LexicalAnalysisException("Found new line inside regular expression literal. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");
                    else if(ch == '\\') {
                        str += ch;
                        column++;

                        if(isAtEnd())
                            throw LexicalAnalysisException("Expecting escape character, encountered end-of-file. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");

                        char escape = this->source[this->index++];
                        str += escape;
                        column++;
                    }
                    else str += ch;
                }

                this->index++;
                column++;

                str = ZhivoUtil::replaceEscapeSequences(std::move(str));
                this->tokens.emplace_back(
                    str,
                    fileName,
                    line,
                    startColumn,
                    TokenType::REGEX
                );
            }
            else {
                std::string op(1, currentChar);
                int startColumn = column;

                while(!isAtEnd() && std::find(
                    OperatorsAndKeys::operators.begin(),
                    OperatorsAndKeys::operators.end(),
                    op + this->source[this->index]) != OperatorsAndKeys::operators.end()
                ) {
                    op += this->source[this->index++];
                    column++;
                }

                this->tokens.emplace_back(
                    op,
                    fileName,
                    line,
                    startColumn,
                    TokenType::OPERATOR
                );
            }
        }
        else if(this->isDigit(currentChar)) {
            std::string digit(1, currentChar);
            int startColumn = column;

            if(currentChar == '0') {
                char nextChar = this->source[this->index++];
                column++;

                switch(nextChar) {
                    case 'b':
                        digit += nextChar;
                        while(!this->isAtEnd() && this->isBinaryDigit(this->source[this->index])) {
                            digit += this->source[this->index++];
                            column++;
                        }
                        break;

                    case 't':
                        digit += nextChar;
                        while(!this->isAtEnd() && this->isTrinaryDigit(this->source[this->index])) {
                            digit += this->source[this->index++];
                            column++;
                        }
                        break;

                    case 'c':
                        digit += nextChar;
                        while(!this->isAtEnd() && this->isOctalDecimalDigit(this->source[this->index])) {
                            digit += this->source[this->index++];
                            column++;
                        }
                        break;

                    case 'x':
                        digit += nextChar;
                        while(!this->isAtEnd() && this->isHexadecimalDigit(this->source[this->index])) {
                            digit += this->source[this->index++];
                            column++;
                        }
                        break;

                    default:
                        this->index--;
                        column--;

                        while(!this->isAtEnd() && this->isDigit(this->source[this->index])) {
                            digit += this->source[this->index++];
                            column++;
                        }

                        if(!this->isAtEnd() && this->source[this->index] == '.') {
                            digit += this->source[this->index++];
                            column++;

                            if(this->isAtEnd() || !this->isDigit(this->source[this->index]))
                                throw LexicalAnalysisException("Expecting decimal digits. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");

                            while(!this->isAtEnd() && this->isDigit(this->source[this->index])) {
                                digit += this->source[this->index++];
                                column++;
                            }
                        }

                        if(!this->isAtEnd() && this->source[this->index] == 'e') {
                            digit += this->source[this->index++];
                            column++;

                            char sign = this->source[this->index++];
                            if(this->isAtEnd() || (sign != '+' && sign != '-'))
                                throw LexicalAnalysisException("Expecting 'e' followed by decimal digits. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");

                            digit += sign;
                            while(!this->isAtEnd() && this->isDigit(this->source[this->index])) {
                                digit += this->source[this->index++];
                                column++;
                            }
                        }
                        break;
                }
            }
            else {
                while(!this->isAtEnd() && this->isDigit(this->source[this->index])) {
                    digit += this->source[this->index++];
                    column++;
                }

                if(!this->isAtEnd() && this->source[this->index] == '.') {
                    digit += this->source[this->index++];
                    column++;

                    if(this->isAtEnd() || !this->isDigit(this->source[this->index]))
                        throw LexicalAnalysisException("Expecting decimal digits. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");

                    while(!this->isAtEnd() && this->isDigit(this->source[this->index])) {
                        digit += this->source[this->index++];
                        column++;
                    }
                }

                if(!this->isAtEnd() && this->source[this->index] == 'e') {
                    digit += this->source[this->index++];
                    column++;

                    char sign = this->source[this->index++];
                    column++;

                    if(this->isAtEnd() || (sign != '+' && sign != '-'))
                        throw LexicalAnalysisException("Expecting 'e' followed by decimal digits. (line " + std::to_string(line) + ", column " + std::to_string(column) + ")");

                    digit += sign;
                    while(!this->isAtEnd() && this->isDigit(this->source[this->index])) {
                        digit += this->source[this->index++];
                        column++;
                    }
                }
            }

            this->tokens.emplace_back(
                digit,
                fileName,
                line,
                startColumn,
                TokenType::DIGIT
            );
        }
        else if(this->isAlphabet(currentChar)) {
            std::string token(1, currentChar);
            int startColumn = column;

            while(!this->isAtEnd() &&
                (this->isDigit(this->source[this->index]) ||
                    this->isAlphabet(this->source[this->index]))
            ) {
                token += this->source[this->index++];
                column++;
            }

            TokenType type = this->isKeyword(token) ?
                TokenType::KEYWORD : TokenType::IDENTIFIER;
            this->tokens.emplace_back(
                token,
                fileName,
                line,
                startColumn,
                type
            );
        }
    }
}

const std::vector<Token>& Tokenizer::getTokens() const {
    return this->tokens;
}

bool Tokenizer::isAtEnd() const {
    return this->index == this->length;
}

bool Tokenizer::isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\f';
}

bool Tokenizer::isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool Tokenizer::isBinaryDigit(char ch) {
    return ch == '0' || ch == '1';
}

bool Tokenizer::isTrinaryDigit(char ch) {
    return ch >= '0' && ch <= '2';
}

bool Tokenizer::isOctalDecimalDigit(char ch) {
    return ch >= '0' && ch <= '7';
}

bool Tokenizer::isHexadecimalDigit(char ch) {
    return (ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'f') ||
        (ch >= 'A' && ch <= 'F');
}

bool Tokenizer::isAlphabet(char ch) {
    return !Tokenizer::isWhitespace(ch) &&
        !Tokenizer::isDigit(ch) &&
        !Tokenizer::isOperator(ch);
}

bool Tokenizer::isOperator(char ch) {
    const std::unordered_set<char> operators = {
        '!', '~', '`', '#', '%', '^', '&', '*',
        '(', ')', '-', '=', '+', '[', ']', '{',
        '}', '|', '"', ':', ';', '<', ',', '>',
        '.', '?', '/'
    };

    return operators.find(ch) != operators.end();
}

bool Tokenizer::isKeyword(const std::string& image) {
    return OperatorsAndKeys::keywords.find(image) !=
        OperatorsAndKeys::keywords.end();
}
