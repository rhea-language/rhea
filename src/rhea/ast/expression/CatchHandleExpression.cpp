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

#include <rhea/ast/expression/CatchHandleExpression.hpp>
#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/core/SymbolTable.hpp>
#include <rhea/parser/Token.hpp>

DynamicObject CatchHandleExpression::visit(SymbolTable& symbols) {
    SymbolTable localTable(symbols);
    DynamicObject object = {};

    try {
        object = this->catchBlock->visit(localTable);
    }
    catch(const TerminativeThrowSignal& throwSig) {
        std::string handleName = this->handler->getImage();
        if(localTable.hasSymbol(handleName))
            throw ASTNodeException(
                std::move(this->address),
                "Handle name for catch-handle is already in-use."
            );

        localTable.setSymbol(
            std::move(this->handler),
            throwSig.getObject()
        );
        object = this->handleBlock->visit(localTable);
    }

    if(this->finalBlock)
        this->finalBlock->visit(localTable);

    return object;
}
