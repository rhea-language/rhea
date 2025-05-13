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

#include <rhea/ast/ASTNode.hpp>
#include <rhea/ast/expression/ArrayAccessExpression.hpp>
#include <rhea/ast/expression/ArrayExpression.hpp>
#include <rhea/ast/expression/BinaryExpression.hpp>
#include <rhea/ast/expression/BlockExpression.hpp>
#include <rhea/ast/expression/BooleanLiteralExpression.hpp>
#include <rhea/ast/expression/CatchHandleExpression.hpp>
#include <rhea/ast/expression/FunctionCallExpression.hpp>
#include <rhea/ast/expression/GroupedExpression.hpp>
#include <rhea/ast/expression/IfElseExpression.hpp>
#include <rhea/ast/expression/LockExpression.hpp>
#include <rhea/ast/expression/LoopExpression.hpp>
#include <rhea/ast/expression/MaybeExpression.hpp>
#include <rhea/ast/expression/NilCoalescingExpression.hpp>
#include <rhea/ast/expression/NilLiteralExpression.hpp>
#include <rhea/ast/expression/NumberLiteralExpression.hpp>
#include <rhea/ast/expression/ParallelExpression.hpp>
#include <rhea/ast/expression/RandomExpression.hpp>
#include <rhea/ast/expression/RegexExpression.hpp>
#include <rhea/ast/expression/RenderExpression.hpp>
#include <rhea/ast/expression/SingleStatementExpression.hpp>
#include <rhea/ast/expression/SizeExpression.hpp>
#include <rhea/ast/expression/StringLiteralExpression.hpp>
#include <rhea/ast/expression/TypeExpression.hpp>
#include <rhea/ast/expression/UnaryExpression.hpp>
#include <rhea/ast/expression/UnlessExpression.hpp>
#include <rhea/ast/expression/VariableAccessExpression.hpp>
#include <rhea/ast/expression/VariableDeclarationExpression.hpp>
#include <rhea/ast/expression/WhenExpression.hpp>
#include <rhea/ast/expression/WhileExpression.hpp>

#include <rhea/ast/statement/BreakStatement.hpp>
#include <rhea/ast/statement/ContinueStatement.hpp>
#include <rhea/ast/statement/DeleteStatement.hpp>
#include <rhea/ast/statement/EmptyStatement.hpp>
#include <rhea/ast/statement/EnumStatement.hpp>
#include <rhea/ast/statement/HaltStatement.hpp>
#include <rhea/ast/statement/ModStatement.hpp>
#include <rhea/ast/statement/ReturnStatement.hpp>
#include <rhea/ast/statement/TestStatement.hpp>
#include <rhea/ast/statement/ThrowStatement.hpp>
#include <rhea/ast/statement/UseStatement.hpp>
#include <rhea/ast/statement/WaitStatement.hpp>

#include <rhea/core/Runtime.hpp>
#include <rhea/core/SymbolTable.hpp>

#include <rhea/parser/Parser.hpp>
#include <rhea/parser/ParserException.hpp>
#include <rhea/parser/Token.hpp>
#include <rhea/parser/Tokenizer.hpp>

#include <rhea/util/Convert.hpp>

#include <memory>
#include <stdexcept>
#include <vector>

Parser Parser::fromFile(const std::string& fileName) {
    std::shared_ptr<Tokenizer> tokenizer = Tokenizer::loadFile(fileName);
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
        throw ParserException(
            std::make_shared<Token>(this->previous()),
            "Encountered end-of-file."
        );

    return this->tokens[(size_t) this->index];
}

bool Parser::isNext(const std::string& image, TokenCategory type) {
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
        throw ParserException(
            std::make_shared<Token>(this->previous()),
            "Expecting \"" + image +
                "\", encountered end-of-code."
        );

    Token token = this->peek();
    if(token.getImage() != image)
        throw ParserException(
            std::make_shared<Token>(this->previous()),
            "Expecting \"" + image +
                "\", encountered \"" + token.getImage() + "\""
        );

    this->advance();
    return token;
}

Token Parser::consume(TokenCategory type) {
    if(this->isAtEnd())
        throw ParserException(
            std::make_shared<Token>(this->previous()),
            "Expecting token type, encountered end-of-code."
        );

    Token token = this->peek();
    if(token.getType() != type)
        throw ParserException(
            std::make_shared<Token>(this->current()),
            "Expecting " + tokenTypeToString(type) +
                ", encountered " + tokenTypeToString(token.getType())
        );

    this->advance();
    return token;
}

Token Parser::getIdentifier() {
    Token token = this->consume(TokenCategory::IDENTIFIER);
    while(this->isNext(".", TokenCategory::OPERATOR)) {
        this->consume(".");

        token.appendToImage(
            "." + this->consume(TokenCategory::IDENTIFIER)
                .getImage()
        );
    }

    return token;
}

const std::vector<std::shared_ptr<ASTNode>>& Parser::getGlobalStatements() const {
    return this->globalStatements;
}

std::shared_ptr<ASTNode> Parser::exprArray() {
    Token address = this->consume("[");
    std::vector<std::shared_ptr<ASTNode>> expressions;

    while(!this->isNext("]", TokenCategory::OPERATOR)) {
        if(!expressions.empty())
            this->consume(",");

        expressions.push_back(this->expression());
    }

    this->consume("]");
    return std::make_shared<ArrayExpression>(
        std::make_shared<Token>(address),
        std::move(expressions)
    );
}

std::shared_ptr<ASTNode> Parser::exprBlock() {
    Token address = this->consume(TokenCategory::OPERATOR);
    std::vector<std::shared_ptr<ASTNode>> statements;

    while(!this->isNext("}", TokenCategory::OPERATOR)) {
        std::shared_ptr<ASTNode> stmt = this->statement();
        statements.push_back(std::move(stmt));
    }
    this->consume("}");

    return std::make_shared<BlockExpression>(
        std::make_shared<Token>(address),
        std::move(statements)
    );
}

std::shared_ptr<ASTNode> Parser::exprCatchHandle() {
    Token address = this->consume("catch");
    std::shared_ptr<ASTNode> catchExpr = this->expression();
    this->consume("handle");

    Token handler = this->getIdentifier();
    std::shared_ptr<ASTNode> handleExpr = this->expression();

    std::shared_ptr<ASTNode> finalExpr = nullptr;
    if(this->isNext("then", TokenCategory::KEYWORD)) {
        this->consume("then");
        finalExpr = this->expression();
    }

    return std::make_shared<CatchHandleExpression>(
        std::make_shared<Token>(address),
        std::move(catchExpr),
        std::move(handleExpr),
        std::make_shared<Token>(handler),
        std::move(finalExpr)
    );
}

std::shared_ptr<ASTNode> Parser::exprFunctionDecl() {
    Token address = this->consume("func");
    this->consume("(");

    std::vector<std::shared_ptr<Token>> parameters;
    while(!this->isNext(")", TokenCategory::OPERATOR)) {
        if(!parameters.empty())
            this->consume(",");

        parameters.push_back(
            std::make_shared<Token>(
                this->getIdentifier()
            )
        );
    }
    this->consume(")");

    std::shared_ptr<ASTNode> body = this->expression();
    return std::make_shared<FunctionDeclarationExpression>(
        std::make_shared<Token>(address),
        std::move(parameters),
        std::move(body)
    );
}

std::shared_ptr<ASTNode> Parser::exprLoop() {
    Token address = this->consume("loop");
    if(this->isNext("(", TokenCategory::OPERATOR)) {
        this->consume("(");

        std::shared_ptr<ASTNode> initial = this->expression();
        this->consume(";");

        std::shared_ptr<ASTNode> condition = this->expression();
        this->consume(";");

        std::shared_ptr<ASTNode> postexpr = this->expression();
        this->consume(")");

        std::shared_ptr<ASTNode> body = this->expression();
        return std::make_shared<LoopExpression>(
            std::make_shared<Token>(address),
            std::move(initial),
            std::move(condition),
            std::move(postexpr),
            std::move(body)
        );
    }

    return std::make_shared<WhileExpression>(
        std::make_shared<Token>(address),
        std::make_shared<BooleanLiteralExpression>(
            std::make_shared<Token>(address),
            true
        ),
        this->expression()
    );
}

std::shared_ptr<ASTNode> Parser::exprIf() {
    Token address = this->consume("if");
    this->consume("(");

    std::shared_ptr<ASTNode> condition = this->expression();
    this->consume(")");

    std::shared_ptr<ASTNode> thenExpr = this->expression();
    std::shared_ptr<ASTNode> elseExpr = nullptr;

    if(this->isNext("else", TokenCategory::KEYWORD)) {
        this->consume("else");
        elseExpr = this->expression();
    }

    return std::make_shared<IfElseExpression>(
        std::make_shared<Token>(address),
        std::move(condition),
        std::move(thenExpr),
        std::move(elseExpr)
    );
}

std::shared_ptr<ASTNode> Parser::exprLiteral() {
    std::shared_ptr<ASTNode> expr = nullptr;

    if(this->isNext("true", TokenCategory::KEYWORD))
        expr = std::make_shared<BooleanLiteralExpression>(
            std::make_shared<Token>(this->consume("true")),
            true
        );
    else if(this->isNext("false", TokenCategory::KEYWORD))
        expr = std::make_shared<BooleanLiteralExpression>(
            std::make_shared<Token>(this->consume("false")),
            false
        );
    else if(this->isNext("maybe", TokenCategory::KEYWORD))
        expr = std::make_shared<MaybeExpression>(
            std::make_shared<Token>(this->consume("maybe"))
        );
    else if(this->isNext("nil", TokenCategory::KEYWORD))
        expr = std::make_shared<NilLiteralExpression>(
            std::make_shared<Token>(this->consume("nil"))
        );
    else if(!this->isAtEnd() && this->peek().getType() == TokenCategory::STRING) {
        Token stringToken = this->consume(TokenCategory::STRING);
        expr = std::make_shared<StringLiteralExpression>(
            std::make_shared<Token>(stringToken),
            stringToken.getImage()
        );
    }
    else if(!this->isAtEnd() && this->peek().getType() == TokenCategory::DIGIT) {
        Token digitToken = this->consume(TokenCategory::DIGIT);
        expr = std::make_shared<NumberLiteralExpression>(
            std::make_shared<Token>(digitToken),
            RheaUtil::Convert::translateDigit(digitToken.getImage())
        );
    }
    else if(!this->isAtEnd() && this->peek().getType() == TokenCategory::REGEX) {
        Token regexToken = this->consume(TokenCategory::REGEX);
        std::string regExpression(regexToken.getImage());

        expr = std::make_shared<RegexExpression>(
            std::make_shared<Token>(regexToken),
            regExpression
        );
    }
    else if(!this->isAtEnd() && this->peek().getType() == TokenCategory::IDENTIFIER) {
        Token var = this->getIdentifier();
        expr = std::make_shared<VariableAccessExpression>(
            std::make_shared<Token>(var)
        );

        while(this->isNext("[", TokenCategory::OPERATOR)) {
            Token address = this->consume("[");
            std::shared_ptr<ASTNode> indexExpr = this->expression();

            this->consume("]");
            expr = std::make_shared<ArrayAccessExpression>(
                std::make_shared<Token>(address),
                std::move(expr),
                std::move(indexExpr)
            );
        }
    }

    if(!expr) {
        auto address = this->current();
        throw ParserException(
            std::make_shared<Token>(address),
            "Expecting expression, encountered " +
                address.getImage()
        );
    }

    return expr;
}

std::shared_ptr<ASTNode> Parser::exprRandom() {
    Token address = this->consume("random");
    std::shared_ptr<ASTNode> thenExpr = this->expression();
    std::shared_ptr<ASTNode> elseExpr = nullptr;

    if(this->isNext("else", TokenCategory::KEYWORD)) {
        this->consume("else");
        elseExpr = this->expression();
    }

    return std::make_shared<RandomExpression>(
        std::make_shared<Token>(address),
        std::move(thenExpr),
        std::move(elseExpr)
    );
}

std::shared_ptr<ASTNode> Parser::exprParallel() {
    Token address = this->consume("parallel");
    std::shared_ptr<ASTNode> expression = this->expression();

    return std::make_shared<ParallelExpression>(
        std::make_shared<Token>(address),
        std::move(expression)
    );
}

std::shared_ptr<ASTNode> Parser::exprRender() {
    Token address = this->consume("render");
    bool newLine = false, errorStream = false;

    if(this->isNext("!", TokenCategory::OPERATOR)) {
        this->consume("!");
        newLine = true;
    }

    if(this->isNext("%", TokenCategory::OPERATOR)) {
        this->consume("%");
        errorStream = true;
    }

    std::shared_ptr<ASTNode> expression = this->expression();
    return std::make_shared<RenderExpression>(
        std::make_shared<Token>(address),
        newLine,
        errorStream,
        std::move(expression)
    );
}

std::shared_ptr<ASTNode> Parser::exprSingleStatement() {
    Token address = this->consume("@");
    std::shared_ptr<ASTNode> stmt = this->statement();

    return std::make_shared<SingleStatementExpression>(
        std::make_shared<Token>(address),
        std::move(stmt)
    );
}

std::shared_ptr<ASTNode> Parser::exprSize() {
    Token address = this->consume("size");
    std::shared_ptr<ASTNode> expression = this->expression();

    return std::make_shared<SizeExpression>(
        std::make_shared<Token>(address),
        std::move(expression)
    );
}

std::shared_ptr<ASTNode> Parser::exprLock() {
    Token address = this->consume("lock");
    this->consume("(");

    Token variable = this->getIdentifier();
    this->consume(")");

    std::shared_ptr<ASTNode> expression = this->expression();
    return std::make_shared<LockExpression>(
        std::make_shared<Token>(address),
        std::make_shared<Token>(variable),
        std::move(expression)
    );
}

std::shared_ptr<ASTNode> Parser::exprType() {
    Token address = this->consume("type");
    std::shared_ptr<ASTNode> expression = this->expression();

    return std::make_shared<TypeExpression>(
        std::make_shared<Token>(address),
        std::move(expression)
    );
}

std::shared_ptr<ASTNode> Parser::exprUnless() {
    Token address = this->consume("unless");
    this->consume("(");

    std::shared_ptr<ASTNode> condition = this->expression();
    this->consume(")");

    std::shared_ptr<ASTNode> thenExpr = this->expression();
    std::shared_ptr<ASTNode> elseExpr = nullptr;

    if(this->isNext("else", TokenCategory::KEYWORD)) {
        this->consume("else");
        elseExpr = this->expression();
    }

    return std::make_shared<UnlessExpression>(
        std::make_shared<Token>(address),
        std::move(condition),
        std::move(thenExpr),
        std::move(elseExpr)
    );
}

std::shared_ptr<ASTNode> Parser::exprWhen() {
    Token address = this->consume("when");
    this->consume("(");

    std::shared_ptr<ASTNode> expression = this->expression();
    this->consume(")");
    this->consume("{");

    std::vector<std::pair<std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>> cases;
    std::shared_ptr<ASTNode> defaultCase = nullptr;

    while(!this->isNext("}", TokenCategory::OPERATOR)) {
        if(!cases.empty())
            this->consume(",");

        if(this->isNext("if", TokenCategory::KEYWORD)) {
            this->consume("if");
            this->consume("(");

            std::shared_ptr<ASTNode> caseExpr = this->expression();
            this->consume(")");

            std::shared_ptr<ASTNode> thenBlock = this->expression();
            cases.push_back(std::make_pair(
                std::move(caseExpr),
                std::move(thenBlock)
            ));
        }
        else if(this->isNext("else", TokenCategory::KEYWORD)) {
            if(defaultCase)
                throw ParserException(
                    std::make_shared<Token>(address),
                    "Cannot have more than one (1) else for when expression."
                );

            this->consume("else");
            defaultCase = this->expression();
        }
    }

    this->consume("}");
    return std::make_shared<WhenExpression>(
        std::make_shared<Token>(address),
        std::move(expression),
        std::move(cases),
        std::move(defaultCase)
    );
}

std::shared_ptr<ASTNode> Parser::exprWhile() {
    Token address = this->consume("while");
    this->consume("(");

    std::shared_ptr<ASTNode> condition = this->expression();
    this->consume(")");

    return std::make_shared<WhileExpression>(
        std::make_shared<Token>(address),
        std::move(condition),
        this->expression()
    );
}

std::shared_ptr<ASTNode> Parser::exprPrimary() {
    std::shared_ptr<ASTNode> expression = nullptr;

    if(this->isNext("+", TokenCategory::OPERATOR) ||
        this->isNext("-", TokenCategory::OPERATOR) ||
        this->isNext("~", TokenCategory::OPERATOR) ||
        this->isNext("!", TokenCategory::OPERATOR)
    ) {
        Token address = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<UnaryExpression>(
            std::make_shared<Token>(address),
            std::string(address.getImage()),
            this->expression()
        );
    }
    else if(this->isNext("(", TokenCategory::OPERATOR)) {
        Token address = this->consume("(");
        std::shared_ptr<ASTNode> innerExpr = this->expression();

        expression = std::make_shared<GroupedExpression>(
            std::make_shared<Token>(address),
            std::move(innerExpr)
        );
        this->consume(")");
    }
    else if(this->isNext("@", TokenCategory::OPERATOR))
        expression = this->exprSingleStatement();
    else if(this->isNext("{", TokenCategory::OPERATOR))
        expression = this->exprBlock();
    else if(this->isNext("render", TokenCategory::KEYWORD))
        expression = this->exprRender();
    else if(this->isNext("catch", TokenCategory::KEYWORD))
        expression = this->exprCatchHandle();
    else if(this->isNext("if", TokenCategory::KEYWORD))
        expression = this->exprIf();
    else if(this->isNext("while", TokenCategory::KEYWORD))
        expression = this->exprWhile();
    else if(this->isNext("loop", TokenCategory::KEYWORD))
        expression = this->exprLoop();
    else if(this->isNext("unless", TokenCategory::KEYWORD))
        expression = this->exprUnless();
    else if(this->isNext("random", TokenCategory::KEYWORD))
        expression = this->exprRandom();
    else if(this->isNext("when", TokenCategory::KEYWORD))
        expression = this->exprWhen();
    else if(this->isNext("func", TokenCategory::KEYWORD))
        expression = this->exprFunctionDecl();
    else if(this->isNext("type", TokenCategory::KEYWORD))
        expression = this->exprType();
    else if(this->isNext("size", TokenCategory::KEYWORD))
        expression = this->exprSize();
    else if(this->isNext("parallel", TokenCategory::KEYWORD))
        expression = this->exprParallel();
    else if(this->isNext("lock", TokenCategory::KEYWORD))
        expression = this->exprLock();
    else if(this->isNext("val", TokenCategory::KEYWORD))
        expression = this->exprVal();
    else if(this->isNext("[", TokenCategory::OPERATOR))
        expression = this->exprArray();
    else if(!this->isAtEnd() && this->peek().getType() == TokenCategory::IDENTIFIER)
        expression = std::make_shared<VariableAccessExpression>(
            std::make_shared<Token>(this->getIdentifier())
        );
    else expression = this->exprLiteral();

    while(this->isNext("(", TokenCategory::OPERATOR) ||
        this->isNext("[", TokenCategory::OPERATOR)) {
        while(this->isNext("(", TokenCategory::OPERATOR)) {
            Token address = this->consume("(");
            std::vector<std::shared_ptr<ASTNode>> arguments;

            while(!this->isNext(")", TokenCategory::OPERATOR)) {
                if(!arguments.empty())
                    this->consume(",");

                arguments.push_back(this->expression());
            }

            this->consume(")");
            expression = std::make_shared<FunctionCallExpression>(
                std::move(expression->getAddress()),
                std::move(expression),
                std::move(arguments)
            );
        }

        while(this->isNext("[", TokenCategory::OPERATOR)) {
            Token address = this->consume("[");
            std::shared_ptr<ASTNode> indexExpr = this->expression();

            this->consume("]");
            expression = std::make_shared<ArrayAccessExpression>(
                std::make_shared<Token>(address),
                std::move(expression),
                std::move(indexExpr)
            );
        }
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprLogicOr() {
    std::shared_ptr<ASTNode> expression = this->exprLogicAnd();

    while(this->isNext("||", TokenCategory::OPERATOR)) {
        Token address = this->consume("||");
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(address),
            std::move(expression),
            "||",
            this->exprLogicAnd()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprLogicAnd() {
    std::shared_ptr<ASTNode> expression = this->exprBitwiseOr();

    while(this->isNext("&&", TokenCategory::OPERATOR)) {
        Token address = this->consume("&&");
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(address),
            std::move(expression),
            this->consume(TokenCategory::OPERATOR).getImage(),
            this->exprBitwiseOr()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprBitwiseOr() {
    std::shared_ptr<ASTNode> expression = this->exprBitwiseXor();

    while(this->isNext("|", TokenCategory::OPERATOR) ||
        this->isNext(".|", TokenCategory::OPERATOR)) {
        Token address = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(address),
            std::move(expression),
            address.getImage(),
            this->exprBitwiseXor()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprBitwiseXor() {
    std::shared_ptr<ASTNode> expression = this->exprBitwiseAnd();

    while(this->isNext("^", TokenCategory::OPERATOR) ||
        this->isNext(".^", TokenCategory::OPERATOR)) {
        Token address = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(address),
            std::move(expression),
            address,
            this->exprBitwiseAnd()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprBitwiseAnd() {
    std::shared_ptr<ASTNode> expression = this->exprNilCoalescing();

    while(this->isNext("&", TokenCategory::OPERATOR) ||
        this->isNext(".&", TokenCategory::OPERATOR)) {
        Token address = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(address),
            std::move(expression),
            address.getImage(),
            this->exprNilCoalescing()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprNilCoalescing() {
    std::shared_ptr<ASTNode> expression = this->exprEquality();

    while(this->isNext("?", TokenCategory::OPERATOR)) {
        Token address = this->consume("?");
        expression = std::make_shared<NilCoalescingExpression>(
            std::make_shared<Token>(address),
            std::move(expression),
            this->exprEquality()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprEquality() {
    std::shared_ptr<ASTNode> expression = this->exprComparison();

    while(this->isNext("==", TokenCategory::OPERATOR) ||
        this->isNext("!=", TokenCategory::OPERATOR) ||
        this->isNext("=", TokenCategory::OPERATOR) ||
        this->isNext("::", TokenCategory::OPERATOR) ||
        this->isNext("!:", TokenCategory::OPERATOR)) {
        Token op = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(op),
            std::move(expression),
            op.getImage(),
            this->exprComparison()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprComparison() {
    std::shared_ptr<ASTNode> expression = this->exprShift();

    while(this->isNext("<", TokenCategory::OPERATOR) ||
        this->isNext("<=", TokenCategory::OPERATOR) ||
        this->isNext(">", TokenCategory::OPERATOR) ||
        this->isNext(">=", TokenCategory::OPERATOR)
    ) {
        Token op = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(op),
            std::move(expression),
            op.getImage(),
            this->exprShift()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprShift() {
    std::shared_ptr<ASTNode> expression = this->exprTerm();

    while(this->isNext("<<", TokenCategory::OPERATOR) ||
        this->isNext(">>", TokenCategory::OPERATOR) ||
        this->isNext(".<<", TokenCategory::OPERATOR) ||
        this->isNext(".>>", TokenCategory::OPERATOR)) {
        Token op = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(op),
            std::move(expression),
            op.getImage(),
            this->exprTerm()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprTerm() {
    std::shared_ptr<ASTNode> expression = this->exprFactor();

    while(this->isNext("+", TokenCategory::OPERATOR) ||
        this->isNext("-", TokenCategory::OPERATOR) ||
        this->isNext(".+", TokenCategory::OPERATOR) ||
        this->isNext(".-", TokenCategory::OPERATOR)) {
        Token op = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(op),
            std::move(expression),
            op.getImage(),
            this->exprFactor()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprFactor() {
    std::shared_ptr<ASTNode> expression = this->exprPrimary();

    while(this->isNext("*", TokenCategory::OPERATOR) ||
        this->isNext("/", TokenCategory::OPERATOR) ||
        this->isNext("\\", TokenCategory::OPERATOR) ||
        this->isNext("%", TokenCategory::OPERATOR) ||
        this->isNext(".*", TokenCategory::OPERATOR) ||
        this->isNext("./", TokenCategory::OPERATOR) ||
        this->isNext(".%", TokenCategory::OPERATOR)) {
        Token op = this->consume(TokenCategory::OPERATOR);
        expression = std::make_shared<BinaryExpression>(
            std::make_shared<Token>(op),
            std::move(expression),
            op.getImage(),
            this->exprPrimary()
        );
    }

    return expression;
}

std::shared_ptr<ASTNode> Parser::exprVal() {
    std::string nativePath = "";
    Token address = this->consume("val");

    if(this->isNext("(", TokenCategory::OPERATOR)) {
        this->consume("(");
        nativePath = this->consume(TokenCategory::STRING).getImage();

        this->consume(")");
    }

    std::map<
        Token,
        std::pair<
            std::vector<std::string>,
            std::shared_ptr<ASTNode>
        >
    > declarations;
    std::vector<std::string> platform;

    if(this->isNext("[", TokenCategory::OPERATOR)) {
        this->consume("[");

        while(!this->isAtEnd()) {
            Token os = this->consume(TokenCategory::STRING);
            platform.emplace_back(os.getImage());

            if(!this->isNext("]", TokenCategory::OPERATOR))
                this->consume(",");
            else break;
        }

        this->consume("]");
    }

    while(true) {
        if(!declarations.empty())
            this->consume(",");

        std::shared_ptr<ASTNode> value;
        Token variable = this->getIdentifier();

        if(nativePath == "") {
            this->consume("=");
            value = this->expression();
        }
        else value = std::make_shared<NilLiteralExpression>(
            std::make_shared<Token>(variable)
        );

        declarations.insert({
            variable,
            std::make_pair(
                platform,
                std::move(value)
            )
        });

        if(!this->isNext(",", TokenCategory::OPERATOR))
            break;
    }

    return std::make_shared<VariableDeclarationExpression>(
        std::make_shared<Token>(address),
        std::move(declarations),
        nativePath
    );
}

std::shared_ptr<ASTNode> Parser::expression() {
    return this->exprLogicOr();
}

std::shared_ptr<ASTNode> Parser::stmtBreak() {
    Token address = this->consume("break");

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<BreakStatement>(
        std::make_shared<Token>(address)
    );
}

std::shared_ptr<ASTNode> Parser::stmtContinue() {
    Token address = this->consume("continue");

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<ContinueStatement>(
        std::make_shared<Token>(address)
    );
}

std::shared_ptr<ASTNode> Parser::stmtDelete() {
    Token address = this->consume("delete");
    std::vector<std::shared_ptr<Token>> variables;

    variables.push_back(
        std::make_shared<Token>(
            this->getIdentifier()
        )
    );

    while(this->isNext(",", TokenCategory::OPERATOR)) {
        this->consume(",");
        variables.push_back(
            std::make_shared<Token>(
                this->getIdentifier()
            )
        );
    }

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<DeleteStatement>(
        std::make_shared<Token>(address),
        variables
    );
}

std::shared_ptr<ASTNode> Parser::stmtEnum() {
    Token address = this->consume("enum"),
        name = this->getIdentifier();
    this->consume("{");

    std::map<std::shared_ptr<Token>, std::shared_ptr<ASTNode>> list;
    while(!this->isAtEnd() && !this->isNext("}", TokenCategory::OPERATOR)) {
        if(!list.empty())
            this->consume(",");

        Token item = this->consume(TokenCategory::IDENTIFIER);
        this->consume("=");

        std::shared_ptr<ASTNode> expression = this->expression();
        list.insert({
            std::make_shared<Token>(item),
            std::move(expression)
        });
    }

    this->consume("}");
    return std::make_shared<EnumStatement>(
        std::make_shared<Token>(address),
        std::make_shared<Token>(name),
        std::move(list)
    );
}

std::shared_ptr<ASTNode> Parser::stmtHalt() {
    Token address = this->consume("halt");

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<HaltStatement>(
        std::make_shared<Token>(address)
    );
}

std::shared_ptr<ASTNode> Parser::stmtImport() {
    Token address = this->consume("import");
    std::string name = "";

    if(this->peek().getType() == TokenCategory::IDENTIFIER)
        name = this->getIdentifier().getImage();

    std::map<
        Token,
        std::pair<
            std::vector<std::string>,
            std::shared_ptr<ASTNode>
        >
    > declarations;
    std::vector<std::string> platform;

    if(this->isNext("[", TokenCategory::OPERATOR)) {
        this->consume("[");

        while(!this->isAtEnd()) {
            Token os = this->consume(TokenCategory::STRING);
            platform.emplace_back(os.getImage());

            if(!this->isNext("]", TokenCategory::OPERATOR))
                this->consume(",");
            else break;
        }

        this->consume("]");
    }

    if(this->isNext("{", TokenCategory::OPERATOR)) {
        this->consume("{");

        while(true) {
            if(!declarations.empty())
                this->consume(",");

            Token variable = this->getIdentifier();
            variable.modifyImage(name + "." + variable.getImage());

            if(this->isNext("[", TokenCategory::OPERATOR)) {
                this->consume("[");

                while(!this->isAtEnd()) {
                    Token os = this->consume(TokenCategory::STRING);
                    platform.emplace_back(os.getImage());

                    if(!this->isNext("]", TokenCategory::OPERATOR))
                        this->consume(",");
                    else break;
                }

                this->consume("]");
            }

            declarations.insert({
                variable,
                std::make_pair(
                    platform,
                    std::make_shared<NilLiteralExpression>(
                        std::make_shared<Token>(variable)
                    )
                )
            });

            if(!this->isNext(",", TokenCategory::OPERATOR))
                break;
        }

        this->consume("}");
    }
    else {
        this->consume(":");

        Token variable = this->getIdentifier();
        declarations.insert({
            variable,
            std::make_pair(
                platform,
                std::make_shared<NilLiteralExpression>(
                    std::make_shared<Token>(variable)
                )
            )
        });
    }

    this->consume("from");
    return std::make_shared<VariableDeclarationExpression>(
        std::make_shared<Token>(address),
        std::move(declarations),
        this->consume(TokenCategory::STRING).getImage()
    );
}

std::shared_ptr<ASTNode> Parser::stmtMod() {
    Token address = this->consume("mod"),
        name = this->getIdentifier();
    this->consume("{");

    std::map<std::shared_ptr<Token>, std::shared_ptr<ASTNode>> list;
    while(!this->isAtEnd() && !this->isNext("}", TokenCategory::OPERATOR)) {
        Token item = this->consume(TokenCategory::IDENTIFIER);
        this->consume(":");

        std::shared_ptr<ASTNode> expression = this->expression();
        list.insert({
            std::make_shared<Token>(item),
            std::move(expression)
        });
    }

    this->consume("}");
    return std::make_shared<ModStatement>(
        std::make_shared<Token>(address),
        std::make_shared<Token>(name),
        std::move(list)
    );
}

std::shared_ptr<ASTNode> Parser::stmtRet() {
    Token address = this->consume("ret");
    std::shared_ptr<ASTNode> expression = this->expression();

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<ReturnStatement>(
        std::make_shared<Token>(address),
        std::move(expression)
    );        
}

std::shared_ptr<ASTNode> Parser::stmtThrow() {
    Token address = this->consume("throw");
    std::shared_ptr<ASTNode> expression = this->expression();

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<ThrowStatement>(
        std::make_shared<Token>(address),
        std::move(expression)
    );
}

std::shared_ptr<ASTNode> Parser::stmtTest() {
    Token address = this->consume("test");
    this->consume("(");

    std::shared_ptr<ASTNode> testName = this->expression();
    this->consume(")");

    std::shared_ptr<ASTNode> testAssert = std::make_shared<NilLiteralExpression>(
        std::make_shared<Token>(address)
    );
    if(this->isNext("if", TokenCategory::KEYWORD)) {
        this->consume("if");
        this->consume("(");

        testAssert = this->expression();
        this->consume(")");
    }

    std::shared_ptr<ASTNode> testBody = this->expression();

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    if(Runtime::isTestMode())
        return std::make_shared<TestStatement>(
            std::make_shared<Token>(address),
            std::move(testName),
            std::move(testBody),
            std::move(testAssert)
        );

    return std::make_shared<EmptyStatement>(
        std::make_shared<Token>(address)
    );
}

std::shared_ptr<ASTNode> Parser::stmtUse() {
    Token address = this->consume("use");
    std::shared_ptr<ASTNode> libName = this->expression(),
        libVersion = nullptr;

    if(!this->isAtEnd() && this->peek().getImage() == "@") {
        this->consume("@");
        libVersion = this->expression();
    }
    else libVersion = std::make_shared<StringLiteralExpression>(
        std::make_shared<Token>(address),
        "1.0.0"
    );

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<UseStatement>(
        std::make_shared<Token>(address),
        std::move(libName),
        std::move(libVersion)
    );
}

std::shared_ptr<ASTNode> Parser::stmtWait() {
    Token address = this->consume("wait");

    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return std::make_shared<WaitStatement>(
        std::make_shared<Token>(address)
    );
}

std::shared_ptr<ASTNode> Parser::statement() {
    if(this->isNext("break", TokenCategory::KEYWORD))
        return this->stmtBreak();
    else if(this->isNext("continue", TokenCategory::KEYWORD))
        return this->stmtContinue();
    else if(this->isNext("delete", TokenCategory::KEYWORD))
        return this->stmtDelete();
    else if(this->isNext("enum", TokenCategory::KEYWORD))
        return this->stmtEnum();
    else if(this->isNext("halt", TokenCategory::KEYWORD))
        return this->stmtHalt();
    else if(this->isNext("import", TokenCategory::KEYWORD))
        return this->stmtImport();
    else if(this->isNext("mod", TokenCategory::KEYWORD))
        return this->stmtMod();
    else if(this->isNext("ret", TokenCategory::KEYWORD))
        return this->stmtRet();
    else if(this->isNext("throw", TokenCategory::KEYWORD))
        return this->stmtThrow();
    else if(this->isNext("test", TokenCategory::KEYWORD))
        return this->stmtTest();
    else if(this->isNext("use", TokenCategory::KEYWORD))
        return this->stmtUse();
    else if(this->isNext("wait", TokenCategory::KEYWORD))
        return this->stmtWait();
    else if(this->isNext(";", TokenCategory::OPERATOR))
        return std::make_shared<EmptyStatement>(
            std::make_shared<Token>(this->consume(";"))
        );

    std::shared_ptr<ASTNode> expr = this->expression();
    if(this->isNext(";", TokenCategory::OPERATOR))
        this->consume(";");

    return expr;
}

void Parser::parse() {
    while(!this->isAtEnd())
        this->globalStatements.push_back(this->statement());
}
