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

#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/ast/expression/FunctionCallExpression.hpp>
#include <rhea/core/Runtime.hpp>
#include <rhea/parser/Parser.hpp>

DynamicObject FunctionCallExpression::visit(SymbolTable& symbols) {
    auto func = this->callable->visit(symbols);
    if(!func.isFunction() && !func.isNative())
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
            throw ASTNodeException(
                std::move(this->address),
                "Native function is nil."
            );

        return (*nativeFunc)(
            std::move(this->address),
            symbols,
            args,
            Runtime::isUnsafeMode()
        );
    }

    return caller->call(symbols, args);
}
