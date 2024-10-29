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

#ifndef ZHIVO_HPP
#define ZHIVO_HPP

#include <zhivo/ast/ASTNodeException.hpp>
#include <zhivo/ast/TerminativeSignal.hpp>
#include <zhivo/core/Runtime.hpp>
#include <zhivo/core/SymbolTable.hpp>
#include <zhivo/parser/LexicalAnalysisException.hpp>
#include <zhivo/parser/Parser.hpp>
#include <zhivo/parser/ParserException.hpp>
#include <zhivo/util/ArgumentParser.hpp>

#define ZHIVO_BUILD_TYPE        "beta"
#define ZHIVO_VERSION           "v1.0.0-" ZHIVO_BUILD_TYPE

#define ZHIVO_BUILD_TIME        __TIME__ " " __DATE__

#endif
