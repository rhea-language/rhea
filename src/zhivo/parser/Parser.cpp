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

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/ast/expression/ArrayAccessExpression.hpp>
#include <zhivo/ast/expression/ArrayExpression.hpp>
#include <zhivo/ast/expression/BinaryExpression.hpp>
#include <zhivo/ast/expression/BlockExpression.hpp>
#include <zhivo/ast/expression/BooleanLiteralExpression.hpp>
#include <zhivo/ast/expression/CatchHandleExpression.hpp>
#include <zhivo/ast/expression/FunctionCallExpression.hpp>
#include <zhivo/ast/expression/GroupedExpression.hpp>
#include <zhivo/ast/expression/IfElseExpression.hpp>
#include <zhivo/ast/expression/LoopExpression.hpp>
#include <zhivo/ast/expression/MaybeExpression.hpp>
#include <zhivo/ast/expression/NilCoalescingExpression.hpp>
#include <zhivo/ast/expression/NilLiteralExpression.hpp>
#include <zhivo/ast/expression/NumberLiteralExpression.hpp>
#include <zhivo/ast/expression/ParallelExpression.hpp>
#include <zhivo/ast/expression/RandomExpression.hpp>
#include <zhivo/ast/expression/RegexExpression.hpp>
#include <zhivo/ast/expression/RenderExpression.hpp>
#include <zhivo/ast/expression/StringLiteralExpression.hpp>
#include <zhivo/ast/expression/TypeExpression.hpp>
#include <zhivo/ast/expression/UnaryExpression.hpp>
#include <zhivo/ast/expression/UnlessExpression.hpp>
#include <zhivo/ast/expression/VariableAccessExpression.hpp>
#include <zhivo/ast/expression/VariableDeclarationExpression.hpp>
#include <zhivo/ast/expression/WhenExpression.hpp>
#include <zhivo/ast/expression/WhileExpression.hpp>

#include <zhivo/ast/statement/BreakStatement.hpp>
#include <zhivo/ast/statement/ContinueStatement.hpp>
#include <zhivo/ast/statement/ReturnStatement.hpp>
#include <zhivo/ast/statement/TestStatement.hpp>
#include <zhivo/ast/statement/ThrowStatement.hpp>
#include <zhivo/ast/statement/WaitStatement.hpp>

#include <zhivo/core/SymbolTable.hpp>

#include <zhivo/parser/Parser.hpp>
#include <zhivo/parser/ParserException.hpp>
#include <zhivo/parser/Token.hpp>
#include <zhivo/parser/Tokenizer.hpp>

#include <zhivo/util/Convert.hpp>

#include <memory>
#include <stdexcept>
#include <vector>

Parser Parser::fromFile(const std::string& fileName) {
    std::unique_ptr<Tokenizer> tokenizer = Tokenizer::loadFile(fileName);
    tokenizer->scan();

    return Parser(tokenizer->getTokens());
}

bool Parser::isAtEnd() const {
    return this->index == this->length;
}

void Parser::advance() {
    this->index++;
}

Token Parser::previous() {
    if(this->index > 1)
        return this->tokens[(size_t) this->index - 1];

    return this->tokens[0];
}

Token Parser::peek() {
    if(this->isAtEnd())
        #ifdef _MSC_VER
        #   pragma warning(disable : 5272)
        #endif
        throw ParserException(
            std::make_unique<Token>(this->previous()),
            "Encountered end-of-file."
        );

    return this->tokens[(size_t) this->index];
}

bool Parser::isNext(const std::string& image, TokenType type) {
    if(this->isAtEnd())
        return false;

    Token next = this->peek();
    return next.getImage() == image &&
        next.getType() == type;
}

Token Parser::current() {
    if(this->index >= (int) this->tokens.size())
        return this->previous();

    return this->tokens[(size_t) this->index];
}

Token Parser::consume(const std::string& image) {
    if(this->isAtEnd())
        #ifdef _MSC_VER
        #   pragma warning(disable : 5272)
        #endif
        throw ParserException(
            std::make_unique<Token>(this->previous()),
            "Expecting \"" + image +
                "\", encountered end-of-code."
        );

    Token token = this->peek();
    if(token.getImage() != image)
        #ifdef _MSC_VER
        #   pragma warning(disable : 5272)
        #endif
        throw ParserException(
            std::make_unique<Token>(this->previous()),
            "Expecting \"" + image +
                "\", encountered \"" + token.getImage() + "\""
        );

    this->advance();
    return token;
}

Token Parser::consume(TokenType type) {
    if(this->isAtEnd())
        #ifdef _MSC_VER
        #   pragma warning(disable : 5272)
        #endif
        throw ParserException(
            std::make_unique<Token>(this->previous()),
            "Expecting token type, encountered end-of-code."
        );

    Token token = this->peek();
    if(token.getType() != type)
        #ifdef _MSC_VER
        #   pragma warning(disable : 5272)
        #endif
        throw ParserException(
            std::make_unique<Token>(this->current()),
            "Expecting " + tokenTypeToString(type) +
                ", encountered " + tokenTypeToString(token.getType())
        );

    this->advance();
    return token;
}

const std::vector<std::unique_ptr<ASTNode>>& Parser::getGlobalStatements() const {
    return this->globalStatements;
}

std::unique_ptr<ASTNode> Parser::exprArray() {
    Token address = this->consume("[");
    std::vector<std::unique_ptr<ASTNode>> expressions;

    while(!this->isNext("]", TokenType::OPERATOR)) {
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

std::unique_ptr<ASTNode> Parser::exprBlock() {
    Token address = this->consume("{");
    std::vector<std::unique_ptr<ASTNode>> body;

    while(!this->isNext("}", TokenType::OPERATOR))
        body.emplace_back(this->expression());

    return std::make_unique<BlockExpression>(
        std::make_unique<Token>(address),
        std::move(body)
    );
}

std::unique_ptr<ASTNode> Parser::exprCatchHandle() {
    Token address = this->consume("catch");
    std::unique_ptr<ASTNode> catchExpr = this->expression();
    this->consume("handle");

    Token handler = this->consume(TokenType::IDENTIFIER);
    std::unique_ptr<ASTNode> handleExpr = this->expression();

    std::unique_ptr<ASTNode> finalExpr = nullptr;
    if(this->isNext("then", TokenType::KEYWORD)) {
        this->consume("then");
        finalExpr = this->expression();
    }

    return std::make_unique<CatchHandleExpression>(
        std::make_unique<Token>(address),
        std::move(catchExpr),
        std::move(handleExpr),
        std::make_unique<Token>(handler),
        std::move(finalExpr)
    );
}

std::unique_ptr<ASTNode> Parser::exprFunctionDecl() {
    Token address = this->consume("func");
    this->consume("(");

    std::vector<std::unique_ptr<Token>> parameters;
    while(!this->isNext(")", TokenType::OPERATOR)) {
        if(!parameters.empty())
            this->consume(",");

        parameters.emplace_back(
            std::make_unique<Token>(
                this->consume(TokenType::IDENTIFIER)
            )
        );
    }
    this->consume(")");

    std::unique_ptr<ASTNode> body = this->expression();
    return std::make_unique<FunctionDeclarationExpression>(
        std::make_unique<Token>(address),
        std::move(parameters),
        std::move(body)
    );
}

std::unique_ptr<ASTNode> Parser::exprLoop() {
    Token address = this->consume("loop");
    this->consume("(");

    std::unique_ptr<ASTNode> initial = this->expression();
    this->consume(";");

    std::unique_ptr<ASTNode> condition = this->expression();
    this->consume(";");

    std::unique_ptr<ASTNode> postexpr = this->expression();
    this->consume(")");

    std::unique_ptr<ASTNode> body = this->expression();
    return std::make_unique<LoopExpression>(
        std::make_unique<Token>(address),
        std::move(initial),
        std::move(condition),
        std::move(postexpr),
        std::move(body)
    );
}

std::unique_ptr<ASTNode> Parser::exprIf() {
    Token address = this->consume("if");
    this->consume("(");

    std::unique_ptr<ASTNode> condition = this->expression();
    this->consume(")");

    std::unique_ptr<ASTNode> thenExpr = this->expression();
    std::unique_ptr<ASTNode> elseExpr = nullptr;

    if(this->isNext("else", TokenType::KEYWORD)) {
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

std::unique_ptr<ASTNode> Parser::exprLiteral() {
    std::unique_ptr<ASTNode> expr = nullptr;

    if(this->isNext("true", TokenType::KEYWORD))
        expr = std::make_unique<BooleanLiteralExpression>(
            std::make_unique<Token>(this->consume("true")),
            true
        );
    else if(this->isNext("false", TokenType::KEYWORD))
        expr = std::make_unique<BooleanLiteralExpression>(
            std::make_unique<Token>(this->consume("false")),
            false
        );
    else if(this->isNext("maybe", TokenType::KEYWORD))
        expr = std::make_unique<MaybeExpression>(
            std::make_unique<Token>(this->consume("maybe"))
        );
    else if(this->isNext("nil", TokenType::KEYWORD))
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
    else if(this->peek().getType() == TokenType::REGEX) {
        Token regexToken = this->consume(TokenType::REGEX);
        std::string regExpression(regexToken.getImage());

        expr = std::make_unique<RegexExpression>(
            std::make_unique<Token>(regexToken),
            regExpression
        );
    }
    else if(this->peek().getType() == TokenType::IDENTIFIER) {
        Token var = this->consume(TokenType::IDENTIFIER);
        while(this->isNext(".", TokenType::OPERATOR)) {
            this->consume(".");
            var.appendToImage(
                "." +
                this->consume(TokenType::IDENTIFIER)
                    .getImage()
            );
        }

        expr = std::make_unique<VariableAccessExpression>(
            std::make_unique<Token>(var)
        );

        while(this->isNext("[", TokenType::OPERATOR)) {
            Token address = this->consume("[");
            std::unique_ptr<ASTNode> indexExpr = this->expression();

            this->consume("]");
            expr = std::make_unique<ArrayAccessExpression>(
                std::make_unique<Token>(address),
                std::move(expr),
                std::move(indexExpr)
            );
        }
    }

    if(!expr) {
        auto address = this->current();

        #ifdef _MSC_VER
        #   pragma warning(disable : 5272)
        #endif
        throw ParserException(
            std::make_unique<Token>(address),
            "Expecting expression, encountered " +
                address.getImage()
        );
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::exprRandom() {
    Token address = this->consume("random");
    std::unique_ptr<ASTNode> thenExpr = this->expression();
    std::unique_ptr<ASTNode> elseExpr = nullptr;

    if(this->isNext("else", TokenType::KEYWORD)) {
        this->consume("else");
        elseExpr = this->expression();
    }

    return std::make_unique<RandomExpression>(
        std::make_unique<Token>(address),
        std::move(thenExpr),
        std::move(elseExpr)
    );
}

std::unique_ptr<ASTNode> Parser::exprParallel() {
    Token address = this->consume("parallel");
    std::unique_ptr<ASTNode> expression = this->expression();

    return std::make_unique<ParallelExpression>(
        std::make_unique<Token>(address),
        std::move(expression)
    );
}

std::unique_ptr<ASTNode> Parser::exprRender() {
    Token address = this->consume("render");
    bool newLine = false, errorStream = false;

    if(this->isNext("!", TokenType::OPERATOR)) {
        this->consume("!");
        newLine = true;
    }

    if(this->isNext("%", TokenType::OPERATOR)) {
        this->consume("%");
        errorStream = true;
    }

    std::unique_ptr<ASTNode> expression = this->expression();
    return std::make_unique<RenderExpression>(
        std::make_unique<Token>(address),
        newLine,
        errorStream,
        std::move(expression)
    );
}

std::unique_ptr<ASTNode> Parser::exprType() {
    Token address = this->consume("type");
    std::unique_ptr<ASTNode> expression = this->expression();

    return std::make_unique<TypeExpression>(
        std::make_unique<Token>(address),
        std::move(expression)
    );
}

std::unique_ptr<ASTNode> Parser::exprUnless() {
    Token address = this->consume("unless");
    this->consume("(");

    std::unique_ptr<ASTNode> condition = this->expression();
    this->consume(")");

    std::unique_ptr<ASTNode> thenExpr = this->expression();
    std::unique_ptr<ASTNode> elseExpr = nullptr;

    if(this->isNext("else", TokenType::KEYWORD)) {
        this->consume("else");
        elseExpr = this->expression();
    }

    return std::make_unique<UnlessExpression>(
        std::make_unique<Token>(address),
        std::move(condition),
        std::move(thenExpr),
        std::move(elseExpr)
    );
}

std::unique_ptr<ASTNode> Parser::exprWhen() {
    Token address = this->consume("when");
    this->consume("(");

    std::unique_ptr<ASTNode> expression = this->expression();
    this->consume(")");
    this->consume("{");

    std::vector<std::pair<std::unique_ptr<ASTNode>, std::unique_ptr<ASTNode>>> cases;
    std::unique_ptr<ASTNode> defaultCase = nullptr;

    while(!this->isNext("}", TokenType::OPERATOR)) {
        if(!cases.empty())
            this->consume(",");

        if(this->isNext("if", TokenType::KEYWORD)) {
            this->consume("if");
            this->consume("(");

            std::unique_ptr<ASTNode> caseExpr = this->expression();
            this->consume(")");

            std::unique_ptr<ASTNode> thenBlock = this->expression();
            cases.emplace_back(std::make_pair(
                std::move(caseExpr),
                std::move(thenBlock)
            ));
        }
        else if(this->isNext("else", TokenType::KEYWORD)) {
            if(defaultCase)
                #ifdef _MSC_VER
                #   pragma warning(disable : 5272)
                #endif
                throw ParserException(
                    std::make_unique<Token>(address),
                    "Cannot have more than one (1) else for when expression."
                );

            this->consume("else");
            defaultCase = this->expression();
        }
    }

    this->consume("}");
    return std::make_unique<WhenExpression>(
        std::make_unique<Token>(address),
        std::move(expression),
        std::move(cases),
        std::move(defaultCase)
    );
}

std::unique_ptr<ASTNode> Parser::exprWhile() {
    Token address = this->consume("while");
    this->consume("(");

    std::unique_ptr<ASTNode> condition = this->expression();
    this->consume(")");

    return std::make_unique<WhileExpression>(
        std::make_unique<Token>(address),
        std::move(condition),
        this->expression()
    );
}

std::unique_ptr<ASTNode> Parser::exprPrimary() {
    std::unique_ptr<ASTNode> expression = nullptr;

    if(this->isNext("+", TokenType::OPERATOR) ||
        this->isNext("-", TokenType::OPERATOR) ||
        this->isNext("~", TokenType::OPERATOR) ||
        this->isNext("!", TokenType::OPERATOR)
    ) {
        Token address = this->consume(TokenType::OPERATOR);
        expression = std::make_unique<UnaryExpression>(
            std::make_unique<Token>(address),
            std::move(std::string(address.getImage())),
            this->expression()
        );
    }
    else if(this->isNext("(", TokenType::OPERATOR)) {
        Token address = this->consume("(");
        std::unique_ptr<ASTNode> innerExpr = this->expression();

        expression = std::make_unique<GroupedExpression>(
            std::make_unique<Token>(address),
            std::move(innerExpr)
        );
        this->consume(")");
    }
    else if(this->isNext("{", TokenType::OPERATOR)) {
        Token address = this->consume(TokenType::OPERATOR);
        std::vector<std::unique_ptr<ASTNode>> statements;

        while(!this->isNext("}", TokenType::OPERATOR)) {
            std::unique_ptr<ASTNode> stmt = this->statement();
            statements.emplace_back(std::move(stmt));
        }
        this->consume("}");

        expression = std::make_unique<BlockExpression>(
            std::make_unique<Token>(address),
            std::move(statements)
        );
    }
    else if(this->isNext("render", TokenType::KEYWORD))
        expression = this->exprRender();
    else if(this->isNext("catch", TokenType::KEYWORD))
        expression = this->exprCatchHandle();
    else if(this->isNext("if", TokenType::KEYWORD))
        expression = this->exprIf();
    else if(this->isNext("while", TokenType::KEYWORD))
        expression = this->exprWhile();
    else if(this->isNext("loop", TokenType::KEYWORD))
        expression = this->exprLoop();
    else if(this->isNext("unless", TokenType::KEYWORD))
        expression = this->exprUnless();
    else if(this->isNext("random", TokenType::KEYWORD))
        expression = this->exprRandom();
    else if(this->isNext("when", TokenType::KEYWORD))
        expression = this->exprWhen();
    else if(this->isNext("func", TokenType::KEYWORD))
        expression = this->exprFunctionDecl();
    else if(this->isNext("type", TokenType::KEYWORD))
        expression = this->exprType();
    else if(this->isNext("parallel", TokenType::KEYWORD))
        expression = this->exprParallel();
    else if(this->isNext("val", TokenType::KEYWORD))
        expression = this->exprVal();
    else if(this->isNext("[", TokenType::OPERATOR))
        expression = this->exprArray();
    else if(this->peek().getType() == TokenType::IDENTIFIER) {
        Token var = this->consume(TokenType::IDENTIFIER);
        while(this->isNext(".", TokenType::OPERATOR)) {
            this->consume(".");
            var.appendToImage(
                "." +
                this->consume(TokenType::IDENTIFIER)
                    .getImage()
            );
        }

        expression = std::make_unique<VariableAccessExpression>(
            std::make_unique<Token>(var)
        );

        while(this->isNext("[", TokenType::OPERATOR)) {
            Token address = this->consume("[");
            std::unique_ptr<ASTNode> indexExpr = this->expression();

            this->consume("]");
            expression = std::make_unique<ArrayAccessExpression>(
                std::make_unique<Token>(address),
                std::move(expression),
                std::move(indexExpr)
            );
        }
    }
    else expression = this->exprLiteral();

    while(this->isNext("(", TokenType::OPERATOR) ||
        this->isNext("[", TokenType::OPERATOR)) {
        while(this->isNext("(", TokenType::OPERATOR)) {
            Token address = this->consume("(");
            std::vector<std::unique_ptr<ASTNode>> arguments;

            while(!this->isNext(")", TokenType::OPERATOR)) {
                if(!arguments.empty())
                    this->consume(",");

                arguments.emplace_back(
                    std::move(this->expression())
                );
            }

            this->consume(")");
            expression = std::make_unique<FunctionCallExpression>(
                std::make_unique<Token>(address),
                std::move(expression),
                std::move(arguments)
            );
        }

        while(this->isNext("[", TokenType::OPERATOR)) {
            Token address = this->consume("[");
            std::unique_ptr<ASTNode> indexExpr = this->expression();

            this->consume("]");
            expression = std::make_unique<ArrayAccessExpression>(
                std::make_unique<Token>(address),
                std::move(expression),
                std::move(indexExpr)
            );
        }
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprLogicOr() {
    std::unique_ptr<ASTNode> expression = this->exprLogicAnd();

    while(this->isNext("||", TokenType::OPERATOR)) {
        Token address = this->consume("||");
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(address),
            std::move(expression),
            "||",
            std::move(this->exprLogicAnd())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprLogicAnd() {
    std::unique_ptr<ASTNode> expression = this->exprBitwiseOr();

    while(this->isNext("&&", TokenType::OPERATOR)) {
        Token address = this->consume("&&");
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(address),
            std::move(expression),
            "&&",
            std::move(this->exprBitwiseOr())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprBitwiseOr() {
    std::unique_ptr<ASTNode> expression = this->exprBitwiseXor();

    while(this->isNext("|", TokenType::OPERATOR)) {
        Token address = this->consume("|");
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(address),
            std::move(expression),
            "|",
            std::move(this->exprBitwiseXor())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprBitwiseXor() {
    std::unique_ptr<ASTNode> expression = this->exprBitwiseAnd();

    while(this->isNext("^", TokenType::OPERATOR)) {
        Token address = this->consume("^");
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(address),
            std::move(expression),
            "^",
            std::move(this->exprBitwiseAnd())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprBitwiseAnd() {
    std::unique_ptr<ASTNode> expression = this->exprNilCoalescing();

    while(this->isNext("&", TokenType::OPERATOR)) {
        Token address = this->consume("&");
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(address),
            std::move(expression),
            "&",
            std::move(this->exprNilCoalescing())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprNilCoalescing() {
    std::unique_ptr<ASTNode> expression = this->exprEquality();

    while(this->isNext("?", TokenType::OPERATOR)) {
        Token address = this->consume("?");
        expression = std::make_unique<NilCoalescingExpression>(
            std::make_unique<Token>(address),
            std::move(expression),
            std::move(this->exprEquality())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprEquality() {
    std::unique_ptr<ASTNode> expression = this->exprComparison();

    while(this->isNext("==", TokenType::OPERATOR) ||
        this->isNext("!=", TokenType::OPERATOR) ||
        this->isNext("=", TokenType::OPERATOR) ||
        this->isNext("::", TokenType::OPERATOR) ||
        this->isNext("!:", TokenType::OPERATOR)) {
        Token op = this->consume(TokenType::OPERATOR);
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(op),
            std::move(expression),
            op.getImage(),
            std::move(this->exprComparison())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprComparison() {
    std::unique_ptr<ASTNode> expression = this->exprShift();

    while(this->isNext("<", TokenType::OPERATOR) ||
        this->isNext("<=", TokenType::OPERATOR) ||
        this->isNext(">", TokenType::OPERATOR) ||
        this->isNext(">=", TokenType::OPERATOR)
    ) {
        Token op = this->consume(TokenType::OPERATOR);
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(op),
            std::move(expression),
            op.getImage(),
            std::move(this->exprShift())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprShift() {
    std::unique_ptr<ASTNode> expression = this->exprTerm();

    while(this->isNext("<<", TokenType::OPERATOR) ||
        this->isNext(">>", TokenType::OPERATOR)) {
        Token op = this->consume(TokenType::OPERATOR);
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(op),
            std::move(expression),
            op.getImage(),
            std::move(this->exprTerm())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprTerm() {
    std::unique_ptr<ASTNode> expression = this->exprFactor();

    while(this->isNext("+", TokenType::OPERATOR) ||
        this->isNext("-", TokenType::OPERATOR)) {
        Token op = this->consume(TokenType::OPERATOR);
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(op),
            std::move(expression),
            op.getImage(),
            std::move(this->exprFactor())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprFactor() {
    std::unique_ptr<ASTNode> expression = this->exprPrimary();

    while(this->isNext("*", TokenType::OPERATOR) ||
        this->isNext("/", TokenType::OPERATOR) ||
        this->isNext("%", TokenType::OPERATOR)) {
        Token op = this->consume(TokenType::OPERATOR);
        expression = std::make_unique<BinaryExpression>(
            std::make_unique<Token>(op),
            std::move(expression),
            op.getImage(),
            std::move(this->exprPrimary())
        );
    }

    return expression;
}

std::unique_ptr<ASTNode> Parser::exprVal() {
    std::string nativePath = "";
    Token address = this->consume("val");

    if(this->isNext("(", TokenType::OPERATOR)) {
        this->consume("(");
        nativePath = this->consume(TokenType::STRING).getImage();

        this->consume(")");
    }

    std::map<Token, std::unique_ptr<ASTNode>> declarations;
    while(!this->isNext(";", TokenType::OPERATOR)) {
        if(!declarations.empty())
            this->consume(",");

        std::unique_ptr<ASTNode> value;
        Token variable = this->consume(TokenType::IDENTIFIER);
        while(this->isNext(".", TokenType::OPERATOR)) {
            this->consume(".");
            variable.appendToImage(
                "." +
                this->consume(TokenType::IDENTIFIER)
                    .getImage()
            );
        }

        if(nativePath == "") {
            this->consume("=");
            value = this->expression();
        }
        else value = std::make_unique<NilLiteralExpression>(
            std::make_unique<Token>(variable)
        );

        declarations.insert({variable, std::move(value)});
    }

    return std::make_unique<VariableDeclarationExpression>(
        std::make_unique<Token>(address),
        std::move(declarations),
        nativePath
    );
}

std::unique_ptr<ASTNode> Parser::expression() {
    return this->exprLogicOr();
}

std::unique_ptr<ASTNode> Parser::stmtBreak() {
    Token address = this->consume("break");

    if(this->isNext(";"))
        this->consume(";");

    return std::make_unique<BreakStatement>(
        std::make_unique<Token>(address)
    );
}

std::unique_ptr<ASTNode> Parser::stmtContinue() {
    Token address = this->consume("continue");

    if(this->isNext(";"))
        this->consume(";");

    return std::make_unique<ContinueStatement>(
        std::make_unique<Token>(address)
    );
}

std::unique_ptr<ASTNode> Parser::stmtRet() {
    Token address = this->consume("ret");
    std::unique_ptr<ASTNode> expression = this->expression();

    if(this->isNext(";"))
        this->consume(";");

    return std::make_unique<ReturnStatement>(
        std::make_unique<Token>(address),
        std::move(expression)
    );        
}

std::unique_ptr<ASTNode> Parser::stmtThrow() {
    Token address = this->consume("throw");
    std::unique_ptr<ASTNode> expression = this->expression();

    if(this->isNext(";"))
        this->consume(";");

    return std::make_unique<ThrowStatement>(
        std::make_unique<Token>(address),
        std::move(expression)
    );
}

std::unique_ptr<ASTNode> Parser::stmtTest() {
    Token address = this->consume("test");
    this->consume("(");

    std::unique_ptr<ASTNode> testName = this->expression();
    this->consume(")");

    std::unique_ptr<ASTNode> testBody = this->expression();

    if(this->isNext(";"))
        this->consume(";");

    return std::make_unique<TestStatement>(
        std::make_unique<Token>(address),
        std::move(testName),
        std::move(testBody)
    );
}

std::unique_ptr<ASTNode> Parser::stmtWait() {
    Token address = this->consume("wait");

    if(this->isNext(";"))
        this->consume(";");

    return std::make_unique<WaitStatement>(
        std::make_unique<Token>(address)
    );
}

std::unique_ptr<ASTNode> Parser::statement() {
    if(this->isNext("break", TokenType::KEYWORD))
        return this->stmtBreak();
    else if(this->isNext("continue", TokenType::KEYWORD))
        return this->stmtContinue();
    else if(this->isNext("ret", TokenType::KEYWORD))
        return this->stmtRet();
    else if(this->isNext("throw", TokenType::KEYWORD))
        return this->stmtThrow();
    else if(this->isNext("test", TokenType::KEYWORD))
        return this->stmtTest();
    else if(this->isNext("wait", TokenType::KEYWORD))
        return this->stmtWait();

    std::unique_ptr<ASTNode> expr = this->expression();

    if(this->isNext(";"))
        this->consume(";");

    return expr;
}

void Parser::parse() {
    while(!this->isAtEnd())
        this->globalStatements.push_back(this->statement());
}
