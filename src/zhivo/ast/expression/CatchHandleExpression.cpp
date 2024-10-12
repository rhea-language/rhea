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

#include <zhivo/ast/expression/CatchHandleExpression.hpp>
#include <zhivo/ast/ASTNodeException.hpp>
#include <zhivo/core/SymbolTable.hpp>
#include <zhivo/parser/Token.hpp>

DynamicObject CatchHandleExpression::visit(SymbolTable& symbols) {
    DynamicObject object = {};
    try {
        object = this->catchBlock->visit(symbols);
    }
    catch(const TerminativeThrowSignal& throwSig) {
        std::string handleName = this->handler->getImage();
        if(symbols.hasSymbol(handleName))
            #ifdef _MSC_VER
            #   pragma warning(disable : 5272)
            #endif
            throw ASTNodeException(
                std::move(this->address),
                "Handle name for catch-handle is already in-use."
            );

        symbols.setSymbol(handleName, throwSig.getObject());
        object = this->handleBlock->visit(symbols);
    }

    if(this->finalBlock)
        this->finalBlock->visit(symbols);

    return object;
}
