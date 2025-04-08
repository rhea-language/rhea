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
#include <rhea/ast/statement/UseStatement.hpp>
#include <rhea/core/Runtime.hpp>
#include <rhea/util/PathHelper.hpp>
#include <rhea/util/SemVer.hpp>

DynamicObject UseStatement::visit(
    SymbolTable& symbols
) {
    #ifndef __EMSCRIPTEN__
    DynamicObject libName = this->libraryName->visit(symbols),
        libVer = this->libraryVersion->visit(symbols);

    std::string lName = libName.toString(),
        lVersion = libVer.toString();

    if(!RheaUtil::SemVer::validateSemVer(lVersion))
        throw ASTNodeException(
            std::move(address),
            "Invalid semantic version '" + lVersion +
                "' for library " + lName
        );

    try {
        if(!RheaUtil::PathHelper::isLibraryInstalled(
            lName, lVersion
        )) throw ASTNodeException(
            std::move(this->address),
            "Library not installed: " +
                lName + "@" + lVersion
        );
    }
    catch(const std::exception& ex) {
        throw ASTNodeException(
            std::move(this->address),
            "Cannot find Rhea module path: " + std::string(ex.what())
        );
    }

    Runtime::interpreter(symbols, RheaUtil::PathHelper::getLibraryFiles(
        lName,
        lVersion
    ));
    #else
    throw ASTNodeException(
        std::move(address),
        "The 'use' statement is not allowed on WebAssembly build type."
    );
    #endif

    return {};
}
