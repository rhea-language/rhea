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

#ifndef ZHIVO_PARSER_HPP
#define ZHIVO_PARSER_HPP

#include <ast/ASTNode.hpp>
#include <ast/expression/ArrayExpression.hpp>
#include <ast/expression/BooleanLiteralExpression.hpp>
#include <ast/expression/IfElseExpression.hpp>
#include <ast/expression/NilLiteralExpression.hpp>
#include <ast/expression/NumberLiteralExpression.hpp>
#include <ast/expression/StringLiteralExpression.hpp>
#include <ast/expression/WhileExpression.hpp>
#include <core/SymbolTable.hpp>
#include <parser/Token.hpp>
#include <parser/Tokenizer.hpp>
#include <util/Convert.hpp>

#include <memory>
#include <stdexcept>
#include <vector>

class ParserException : public std::runtime_error {
public:
    explicit ParserException(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
private:
    std::vector<std::unique_ptr<ASTNode>> globalStatements;
    std::vector<Token> tokens;
    int length;
    int index = 0;

public:
    Parser(const std::vector<Token>& _tokens) :
        globalStatements{},
        tokens(_tokens),
        length(static_cast<int>(_tokens.size())) {}

    static Parser fromFile(const std::string& fileName) {
        std::unique_ptr<Tokenizer> tokenizer = Tokenizer::loadFile(fileName);
        tokenizer->scan();

        return Parser(tokenizer->getTokens());
    }

    bool isAtEnd() const {
        return this->index == this->length;
    }

    void advance() {
        this->index++;
    }

    Token peek() const {
        if(this->isAtEnd())
            throw ParserException("Encountered end-of-file.");

        return this->tokens[this->index];
    }

    bool isNext(const std::string& image) const {
        if(this->isAtEnd())
            return false;

        return this->peek().getImage() == image;
    }

    Token consume(const std::string& image) {
        if(this->isAtEnd())
            throw ParserException("Expecting \"" + image + "\", encountered end-of-code.");

        Token token = this->peek();
        if(token.getImage() != image)
            throw ParserException("Expecting \"" + image + "\", encountered " + token.getImage());

        this->advance();
        return token;
    }

    Token consume(TokenType type) {
        if(this->isAtEnd())
            throw ParserException("Expecting token type, encountered end-of-code.");

        Token token = this->peek();
        if(token.getType() != type)
            throw ParserException(
                "Expecting " + tokenTypeToString(type) +
                ", encountered " + tokenTypeToString(token.getType())
            );

        this->advance();
        return token;
    }

    const std::vector<std::unique_ptr<ASTNode>>& getGlobalStatements() const {
        return this->globalStatements;
    }

    std::unique_ptr<ASTNode> exprLiteral() {
        std::unique_ptr<ASTNode> expr = nullptr;

        if(this->isNext("true"))
            expr = std::make_unique<BooleanLiteralExpression>(
                std::make_unique<Token>(this->consume("true")),
                true
            );
        else if(this->isNext("false"))
            expr = std::make_unique<BooleanLiteralExpression>(
                std::make_unique<Token>(this->consume("false")),
                false
            );
        else if(this->isNext("nil"))
            expr = std::make_unique<NilLiteralExpression>(
                std::make_unique<Token>(this->consume("nil"))
            );
        else if(this->peek().getType() == TokenType::STRING) {
            Token stringToken = this->consume(TokenType::STRING);
            expr = std::make_unique<StringLiteralExpression>(
                std::make_unique<Token>(stringToken),
                stringToken.getImage()
            );
        }
        else if(this->peek().getType() == TokenType::DIGIT) {
            Token digitToken = this->consume(TokenType::DIGIT);
            expr = std::make_unique<NumberLiteralExpression>(
                std::make_unique<Token>(digitToken),
                ZhivoUtil::Convert::translateDigit(digitToken.getImage())
            );
        }

        if(!expr)
            throw ParserException(
                "Expecting expression, encountered " +
                this->peek().getImage()
            );

        return expr;
    }

    std::unique_ptr<ASTNode> exprArray() {
        Token address = this->consume("[");
        std::vector<std::unique_ptr<ASTNode>> expressions;

        while(!this->isNext("]")) {
            if(!expressions.empty())
                this->consume(",");

            expressions.push_back(this->expression());
        }

        this->consume("]");
        return std::make_unique<ArrayExpression>(
            std::make_unique<Token>(address),
            std::move(expressions)
        );
    }

    std::unique_ptr<ASTNode> exprIf() {
        Token address = this->consume("if");
        this->consume("(");

        auto condition = this->expression();
        this->consume(")");

        auto thenExpr = this->expression();
        std::unique_ptr<ASTNode> elseExpr = nullptr;

        if(this->isNext("else")) {
            this->consume("else");
            elseExpr = this->expression();
        }

        return std::make_unique<IfElseExpression>(
            std::make_unique<Token>(address),
            std::move(condition),
            std::move(thenExpr),
            std::move(elseExpr)
        );
    }

    std::unique_ptr<ASTNode> exprWhile() {
        Token address = this->consume("while");
        this->consume("(");

        auto condition = this->expression();
        this->consume(")");

        return std::make_unique<WhileExpression>(
            std::make_unique<Token>(address),
            std::move(condition),
            this->expression()
        );
    }

    std::unique_ptr<ASTNode> expression() {
        return this->exprLiteral();
    }

    std::unique_ptr<ASTNode> statement() {
        if(this->isNext("if"))
            return this->exprIf();
        else if(this->isNext("while"))
            return this->exprWhile();

        auto expr = this->expression();
        this->consume(";");

        return expr;
    }

    void parse() {
        while(!this->isAtEnd())
            this->globalStatements.push_back(this->statement());
    }
};

#endif
