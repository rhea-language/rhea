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

#include <ast/expression/ArrayAccessExpression.hpp>
#include <ast/expression/ArrayExpression.hpp>
#include <ast/expression/BinaryExpression.hpp>
#include <ast/expression/BlockExpression.hpp>
#include <ast/expression/FunctionCallExpression.hpp>
#include <ast/expression/NumberLiteralExpression.hpp>
#include <ast/expression/RenderExpression.hpp>
#include <ast/expression/StringLiteralExpression.hpp>
#include <ast/expression/VariableAccessExpression.hpp>

#include <ast/statement/ReturnStatement.hpp>

#include <core/SymbolTable.hpp>

#include <parser/LexicalAnalysisException.hpp>
#include <parser/Parser.hpp>
#include <parser/Token.hpp>
#include <parser/Tokenizer.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

auto interpreter() -> int {
    SymbolTable globalSymbols;

    auto nil = std::make_unique<Token>(
        "",
        "test.jnt",
        1,
        1,
        TokenType::IDENTIFIER
    );

    // Define the token for the function
    auto myFunc = std::make_unique<Token>(
        "myFunc",
        "test.jnt",
        1,
        1,
        TokenType::IDENTIFIER
    );

    // Create and visit a variable declaration
    auto varDecl1Tok = std::make_unique<Token>("x", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto varDecl = std::make_unique<BinaryExpression>(
        std::move(std::make_unique<Token>(
            "",
            "test.jnt",
            1,
            1,
            TokenType::IDENTIFIER
        )),
        std::move(std::make_unique<VariableAccessExpression>(std::move(varDecl1Tok))),
        "=",
        std::make_unique<NumberLiteralExpression>(
            std::make_unique<Token>(*nil),
            3.1416
        )
    );
    varDecl->visit(globalSymbols);

    // Create and visit a variable access
    auto varAcces1Tok = std::make_unique<Token>("x", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto varAccess = std::make_unique<VariableAccessExpression>(std::move(varAcces1Tok));

    // Create and visit a function declaration
    auto funcBodyStmts = std::vector<std::unique_ptr<ASTNode>>();
    funcBodyStmts.push_back(
        std::make_unique<ReturnStatement>(
            std::make_unique<Token>(*nil),
            std::make_unique<VariableAccessExpression>(
                std::move(std::make_unique<Token>("x", "test.jnt", 1, 1, TokenType::IDENTIFIER))
            )
        )
    );

    auto funcBody = std::make_unique<BlockExpression>(
        std::make_unique<Token>(*nil),
        std::move(funcBodyStmts)
    );
    auto funcDecl = std::make_unique<FunctionDeclarationExpression>(
        std::move(myFunc),
        std::vector<std::unique_ptr<Token>>{},
        std::move(funcBody)
    );

    // Store the function declaration directly
    auto varDecl2Tok = std::make_unique<Token>("myFunc", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto myFuncDecl = std::make_unique<BinaryExpression>(
        std::move(std::make_unique<Token>(
            "",
            "test.jnt",
            1,
            1,
            TokenType::IDENTIFIER
        )),
        std::move(std::make_unique<VariableAccessExpression>(std::move(varDecl2Tok))),
        "=",
        std::move(funcDecl)
    );
    myFuncDecl->visit(globalSymbols);

    {
        // Create and visit a function call
        auto funcCallArgs = std::vector<std::unique_ptr<ASTNode>>{};
        auto funcCall = std::make_unique<FunctionCallExpression>(
            std::make_unique<Token>(*nil),
            "myFunc",
            std::move(funcCallArgs)
        );
        auto funcCallPrint = std::make_unique<RenderExpression>(
            std::make_unique<Token>(*nil),
            std::move(funcCall)
        );
        funcCallPrint->visit(globalSymbols);
        std::cout << std::endl;
    }

    {
        // Create and visit a function call
        auto funcCallArgs = std::vector<std::unique_ptr<ASTNode>>{};
        auto funcCall = std::make_unique<FunctionCallExpression>(
            std::make_unique<Token>(*nil),
            "myFunc",
            std::move(funcCallArgs)
        );
        auto funcCallPrint = std::make_unique<RenderExpression>(
            std::make_unique<Token>(*nil),
            std::move(funcCall)
        );
        funcCallPrint->visit(globalSymbols);
        std::cout << std::endl;
    }

    // Create and visit string and number variables
    auto varDecl3Tok = std::make_unique<Token>("message", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto strDecl = std::make_unique<BinaryExpression>(
        std::move(std::make_unique<Token>(
            "",
            "test.jnt",
            1,
            1,
            TokenType::IDENTIFIER
        )),
        std::move(std::make_unique<VariableAccessExpression>(std::move(varDecl3Tok))),
        "=",
        std::make_unique<StringLiteralExpression>(
            std::make_unique<Token>(*nil),
            "The number is: "
        )
    );
    strDecl->visit(globalSymbols);

    auto varDecl4Tok = std::make_unique<Token>("number", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto numDecl = std::make_unique<BinaryExpression>(
        std::move(std::make_unique<Token>(
            "",
            "test.jnt",
            1,
            1,
            TokenType::IDENTIFIER
        )),
        std::move(std::make_unique<VariableAccessExpression>(std::move(varDecl4Tok))),
        "=",
        std::make_unique<NumberLiteralExpression>(
            std::make_unique<Token>(*nil),
            42
        )
    );
    numDecl->visit(globalSymbols);

    // Create and visit a binary expression
    auto varAcces2Tok = std::make_unique<Token>("message", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto strAccess = std::make_unique<VariableAccessExpression>(
        std::move(varAcces2Tok)
    );
    auto varAcces3Tok = std::make_unique<Token>("number", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto numAccess = std::make_unique<VariableAccessExpression>(
        std::move(varAcces3Tok)
    );

    auto addExpr = std::make_unique<BinaryExpression>(
        std::make_unique<Token>(*nil),
        std::move(strAccess),
        "+",
        std::move(numAccess)
    );

    std::vector<std::unique_ptr<ASTNode>> elements;
    elements.push_back(std::make_unique<StringLiteralExpression>(std::make_unique<Token>(*nil), std::move("Hello")));
    elements.push_back(std::make_unique<StringLiteralExpression>(std::make_unique<Token>(*nil), std::move("Kamusta")));
    elements.push_back(std::make_unique<StringLiteralExpression>(std::make_unique<Token>(*nil), std::move("Marhaba")));

    auto arrayExpr = std::make_unique<ArrayExpression>(
        std::make_unique<Token>(*nil),
        std::move(elements)
    );

    auto varDecl_2Tok = std::make_unique<Token>("array", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto varDecl2 = std::make_unique<BinaryExpression>(
        std::move(std::make_unique<Token>(
            "",
            "test.jnt",
            1,
            1,
            TokenType::IDENTIFIER
        )),
        std::move(std::make_unique<VariableAccessExpression>(std::move(varDecl_2Tok))),
        "=",
        std::move(arrayExpr)
    );
    varDecl2->visit(globalSymbols);

    // Create ArrayAccessExpression for use in both assignments and printing
    auto arrayAccessVar1 = std::make_unique<ArrayAccessExpression>(
        std::make_unique<Token>(*nil),
        std::make_unique<VariableAccessExpression>(
            std::make_unique<Token>("array", "test.jnt", 1, 1, TokenType::IDENTIFIER)
        ),
        std::make_unique<NumberLiteralExpression>(std::make_unique<Token>(*nil), 2.0)
    );

    auto arrayAccessVar2 = std::make_unique<ArrayAccessExpression>(
        std::make_unique<Token>(*nil),
        std::make_unique<VariableAccessExpression>(
            std::make_unique<Token>("array", "test.jnt", 1, 1, TokenType::IDENTIFIER)
        ),
        std::make_unique<NumberLiteralExpression>(std::make_unique<Token>(*nil), 2.0)
    );

    // Create BinaryExpression to set value of array at index 2
    auto setValExpr = std::make_unique<BinaryExpression>(
        std::make_unique<Token>(*nil),
        std::move(arrayAccessVar1),
        "=",
        std::make_unique<NumberLiteralExpression>(std::make_unique<Token>(*nil), 3.14)
    );
    setValExpr->visit(globalSymbols);

    // Print the array element at index 2
    auto varDecl_3Tok = std::make_unique<Token>("array", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto print1Stmt = std::make_unique<RenderExpression>(
        std::make_unique<Token>(*nil),
        std::move(std::make_unique<VariableAccessExpression>(std::move(varDecl_3Tok)))
    );
    print1Stmt->visit(globalSymbols);
    std::cout << std::endl;

    // Print the function declaration
    auto varAcces4Tok = std::make_unique<Token>("myFunc", "test.jnt", 1, 1, TokenType::IDENTIFIER);
    auto funcVarAccess = std::make_unique<VariableAccessExpression>(
        std::move(varAcces4Tok)
    );
    auto printStmt = std::make_unique<RenderExpression>(
        std::make_unique<Token>(*nil),
        std::move(addExpr)
    );

    printStmt->visit(globalSymbols);
    std::cout << std::endl;

    // Tokenizer example
    try {
        std::string sourceCode = R"(
            int main() {
                std::string s = "Hello, World!";
                int x = 0xabcdef;

                if(x >= 10)
                    x = x + 1;
            }
        )";

        auto tokenizer = std::make_unique<Tokenizer>(sourceCode, "example.cpp");
        tokenizer->scan();

        const auto& tokens = tokenizer->getTokens();
        for(const auto& token : tokens)
            std::cout << "Token: " << token.toString() << std::endl;
    }
    catch (const LexicalAnalysisException& e) {
        std::cerr << "Lexical Analysis Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

decltype(interpreter()) main() {
    try {
        SymbolTable symbols;
        Parser parser = Parser::fromFile("test.zhv");
        parser.parse();

        for(const auto& statement : parser.getGlobalStatements()) {
            DynamicObject object = statement->visit(symbols);
            std::cout << "Result: " << object.toString() << std::endl;
        }

        return interpreter();
    }
    catch (const std::exception& exc) {
        std::cerr << "Caught exception: " << exc.what() << std::endl;
    }

    return 0;
}