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

#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/statement/UseStatement.hpp>
#include <n8/core/Runtime.hpp>
#include <n8/util/PathHelper.hpp>
#include <n8/util/SemVer.hpp>

DynamicObject UseStatement::visit(
    SymbolTable& symbols
    #ifndef _MSC_VER
    __attribute__((unused))
    #endif
) {
    #ifndef __EMSCRIPTEN__
    DynamicObject libName = this->libraryName->visit(symbols),
        libVer = this->libraryVersion->visit(symbols);

    std::string lName = libName.toString(),
        lVersion = libVer.toString();

    if(!N8Util::SemVer::validateSemVer(lVersion))
        throw ASTNodeException(
            std::move(address),
            "Invalid semantic version '" + lVersion +
                "' for library " + lName
        );

    if(!N8Util::PathHelper::isLibraryInstalled(
        lName, lVersion
    )) throw ASTNodeException(
        std::move(this->address),
        "Library not installed: " +
            lName + "@" + lVersion
    );

    Runtime::interpreter(symbols, N8Util::PathHelper::getLibraryFiles(
        lName,
        lVersion
    ));
    #else
    throw ASTNodeException(
        std::move(address),
        "The 'use' statement is not allowed on WebAssembly build type."
    );
    #endif

    return DynamicObject();
}
