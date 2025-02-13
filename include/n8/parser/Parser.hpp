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

#ifndef N8_PARSER_HPP
#define N8_PARSER_HPP

#include <n8/ast/ASTNode.hpp>
#include <n8/core/SymbolTable.hpp>
#include <n8/parser/Token.hpp>
#include <n8/parser/TokenType.hpp>

#include <memory>
#include <vector>

class Parser final {
private:
    std::vector<std::shared_ptr<ASTNode>> globalStatements;
    std::vector<Token> tokens;
    int length;
    int index = 0;

    std::shared_ptr<ASTNode> exprArray();
    std::shared_ptr<ASTNode> exprBlock();
    std::shared_ptr<ASTNode> exprCatchHandle();
    std::shared_ptr<ASTNode> exprFunctionDecl();
    std::shared_ptr<ASTNode> exprLoop();
    std::shared_ptr<ASTNode> exprIf();
    std::shared_ptr<ASTNode> exprLiteral();
    std::shared_ptr<ASTNode> exprRandom();
    std::shared_ptr<ASTNode> exprRender();
    std::shared_ptr<ASTNode> exprType();
    std::shared_ptr<ASTNode> exprUnless();
    std::shared_ptr<ASTNode> exprWhen();
    std::shared_ptr<ASTNode> exprWhile();
    std::shared_ptr<ASTNode> exprPrimary();
    std::shared_ptr<ASTNode> exprLogicOr();
    std::shared_ptr<ASTNode> exprLogicAnd();
    std::shared_ptr<ASTNode> exprBitwiseOr();
    std::shared_ptr<ASTNode> exprBitwiseXor();
    std::shared_ptr<ASTNode> exprBitwiseAnd();
    std::shared_ptr<ASTNode> exprNilCoalescing();
    std::shared_ptr<ASTNode> exprEquality();
    std::shared_ptr<ASTNode> exprComparison();
    std::shared_ptr<ASTNode> exprShift();
    std::shared_ptr<ASTNode> exprTerm();
    std::shared_ptr<ASTNode> exprFactor();
    std::shared_ptr<ASTNode> exprParallel();
    std::shared_ptr<ASTNode> exprVal();
    std::shared_ptr<ASTNode> exprSize();
    std::shared_ptr<ASTNode> exprLock();

    std::shared_ptr<ASTNode> stmtBreak();
    std::shared_ptr<ASTNode> stmtContinue();
    std::shared_ptr<ASTNode> stmtDelete();
    std::shared_ptr<ASTNode> stmtEnum();
    std::shared_ptr<ASTNode> stmtHalt();
    std::shared_ptr<ASTNode> stmtImport();
    std::shared_ptr<ASTNode> stmtMod();
    std::shared_ptr<ASTNode> stmtRet();
    std::shared_ptr<ASTNode> stmtUse();
    std::shared_ptr<ASTNode> stmtThrow();
    std::shared_ptr<ASTNode> stmtTest();
    std::shared_ptr<ASTNode> stmtWait();

    std::shared_ptr<ASTNode> expression();
    std::shared_ptr<ASTNode> statement();

    Token previous();
    Token peek();

    Token current();
    Token consume(const std::string& image);
    Token consume(TokenType type);
    Token getIdentifier();

    void advance();
    bool isAtEnd() const;
    bool isNext(const std::string& image, TokenType type);

public:
    Parser(const std::vector<Token>& _tokens) :
        globalStatements{},
        tokens(_tokens),
        length(static_cast<int>(_tokens.size())) {}

    const std::vector<std::shared_ptr<ASTNode>>& getGlobalStatements() const;
    void parse();

    static Parser fromFile(const std::string& fileName);
};

#endif
