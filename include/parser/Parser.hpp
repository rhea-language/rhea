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
#include <ast/expression/ArrayAccessExpression.hpp>
#include <ast/expression/ArrayExpression.hpp>
#include <ast/expression/BinaryExpression.hpp>
#include <ast/expression/BlockExpression.hpp>
#include <ast/expression/BooleanLiteralExpression.hpp>
#include <ast/expression/CatchHandleExpression.hpp>
#include <ast/expression/DoWhileExpression.hpp>
#include <ast/expression/FunctionCallExpression.hpp>
#include <ast/expression/GroupedExpression.hpp>
#include <ast/expression/IfElseExpression.hpp>
#include <ast/expression/LoopExpression.hpp>
#include <ast/expression/MaybeExpression.hpp>
#include <ast/expression/NilCoalescingExpression.hpp>
#include <ast/expression/NilLiteralExpression.hpp>
#include <ast/expression/NumberLiteralExpression.hpp>
#include <ast/expression/RandomExpression.hpp>
#include <ast/expression/RenderExpression.hpp>
#include <ast/expression/StringLiteralExpression.hpp>
#include <ast/expression/TypeExpression.hpp>
#include <ast/expression/UnaryExpression.hpp>
#include <ast/expression/UnlessExpression.hpp>
#include <ast/expression/VariableAccessExpression.hpp>
#include <ast/expression/WhenExpression.hpp>
#include <ast/expression/WhileExpression.hpp>

#include <ast/statement/BreakStatement.hpp>
#include <ast/statement/ContinueStatement.hpp>
#include <ast/statement/ReturnStatement.hpp>
#include <ast/statement/TestStatement.hpp>
#include <ast/statement/ThrowStatement.hpp>

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

    std::unique_ptr<ASTNode> exprArray();
    std::unique_ptr<ASTNode> exprBlock();
    std::unique_ptr<ASTNode> exprCatchHandle();
    std::unique_ptr<ASTNode> exprDoWhile();
    std::unique_ptr<ASTNode> exprFunctionDecl();
    std::unique_ptr<ASTNode> exprLoop();
    std::unique_ptr<ASTNode> exprIf();
    std::unique_ptr<ASTNode> exprLiteral();
    std::unique_ptr<ASTNode> exprRandom();
    std::unique_ptr<ASTNode> exprRender();
    std::unique_ptr<ASTNode> exprType();
    std::unique_ptr<ASTNode> exprUnless();
    std::unique_ptr<ASTNode> exprWhen();
    std::unique_ptr<ASTNode> exprWhile();
    std::unique_ptr<ASTNode> exprPrimary();
    std::unique_ptr<ASTNode> exprLogicOr();
    std::unique_ptr<ASTNode> exprLogicAnd();
    std::unique_ptr<ASTNode> exprBitwiseOr();
    std::unique_ptr<ASTNode> exprBitwiseXor();
    std::unique_ptr<ASTNode> exprBitwiseAnd();
    std::unique_ptr<ASTNode> exprNilCoalescing();
    std::unique_ptr<ASTNode> exprEquality();
    std::unique_ptr<ASTNode> exprComparison();
    std::unique_ptr<ASTNode> exprShift();
    std::unique_ptr<ASTNode> exprTerm();
    std::unique_ptr<ASTNode> exprFactor();

    std::unique_ptr<ASTNode> stmtBreak();
    std::unique_ptr<ASTNode> stmtContinue();
    std::unique_ptr<ASTNode> stmtRet();
    std::unique_ptr<ASTNode> stmtThrow();
    std::unique_ptr<ASTNode> stmtTest();

    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> statement();

    Token peek() const;
    Token consume(const std::string& image);
    Token consume(TokenType type);

    void advance();
    bool isNext(const std::string& image) const;
    bool isAtEnd() const;

public:
    Parser(const std::vector<Token>& _tokens) :
        globalStatements{},
        tokens(_tokens),
        length(static_cast<int>(_tokens.size())) {}

    const std::vector<std::unique_ptr<ASTNode>>& getGlobalStatements() const;
    void parse();

    static Parser fromFile(const std::string& fileName);
};

#endif
