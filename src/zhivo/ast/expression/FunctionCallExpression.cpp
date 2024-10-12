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

#include <zhivo/ast/ASTNodeException.hpp>
#include <zhivo/ast/expression/FunctionCallExpression.hpp>
#include <zhivo/parser/Parser.hpp>

DynamicObject FunctionCallExpression::visit(SymbolTable& symbols) {
    auto func = this->callable->visit(symbols);
    if(!func.isFunction() && !func.isNative())
        #ifdef _MSC_VER
        #   pragma warning(disable : 5272)
        #endif
        throw ASTNodeException(
            std::move(this->address),
            "Expression is not a function."
        );

    auto caller = func.getCallable();
    std::vector<DynamicObject> args;

    for(auto& arg : this->arguments)
        args.push_back(arg->visit(symbols));

    if(func.isNative()) {
        auto nativeFunc = func.getNativeFunction();

        if(nativeFunc == nullptr)
            #ifdef _MSC_VER
            #   pragma warning(disable : 5272)
            #endif
            throw ASTNodeException(
                std::move(this->address),
                "Native function is nil."
            );

        return (*nativeFunc)(symbols, args);
    }

    return caller->call(symbols, args);
}
