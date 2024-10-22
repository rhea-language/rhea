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

#include <zhivo/ast/ASTNode.hpp>
#include <zhivo/core/SymbolTable.hpp>
#include <zhivo/parser/Token.hpp>

#include <memory>
#include <vector>

class Parser final {
private:
    std::vector<std::unique_ptr<ASTNode>> globalStatements;
    std::vector<Token> tokens;
    int length;
    int index = 0;

    std::unique_ptr<ASTNode> exprArray();
    std::unique_ptr<ASTNode> exprBlock();
    std::unique_ptr<ASTNode> exprCatchHandle();
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
    std::unique_ptr<ASTNode> exprParallel();
    std::unique_ptr<ASTNode> exprVal();

    std::unique_ptr<ASTNode> stmtBreak();
    std::unique_ptr<ASTNode> stmtContinue();
    std::unique_ptr<ASTNode> stmtRet();
    std::unique_ptr<ASTNode> stmtThrow();
    std::unique_ptr<ASTNode> stmtTest();
    std::unique_ptr<ASTNode> stmtWait();

    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> statement();

    Token previous();
    Token peek();

    Token current();
    Token consume(const std::string& image);
    Token consume(TokenType type);

    void advance();
    bool isAtEnd() const;
    bool isNext(const std::string& image, TokenType type);

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
