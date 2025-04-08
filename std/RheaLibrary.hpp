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

#ifndef RHEA_STD_LIB_CC
#define RHEA_STD_LIB_CC

#include <rhea/core/DynamicObject.hpp>
#include <rhea/core/Runtime.hpp>
#include <rhea/core/SymbolTable.hpp>
#include <rhea/parser/Token.hpp>

#include <memory>

#define RHEA_LIB_START extern "C" {
#define RHEA_LIB_END   }

#define RHEA_FUNC(funcName)                               \
    DynamicObject funcName(                             \
        std::shared_ptr<Token> address,                 \
        SymbolTable& symtab,                            \
        std::vector<DynamicObject>& args,               \
        bool unsafe                                     \
    )

#define RHEA_FUNC_REQUIRE_UNSAFE                          \
    if(!unsafe)                                          \
        throw TerminativeThrowSignal(                   \
            std::move(address),                         \
            "Function requires unsafe mode turned on"   \
        );

#include <RheaStdlibMaps.hpp>

#endif
