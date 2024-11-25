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

#ifndef N8_HPP
#define N8_HPP

#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/TerminativeSignal.hpp>
#include <n8/core/Runtime.hpp>
#include <n8/core/SymbolTable.hpp>
#include <n8/parser/LexicalAnalysisException.hpp>
#include <n8/parser/Parser.hpp>
#include <n8/parser/ParserException.hpp>
#include <n8/util/ArgumentParser.hpp>

#ifdef __TERMUX__
#   define N8_BUILD_PLATFORM "termux"
#elif defined(__linux__)
#   define N8_BUILD_PLATFORM "linux"
#elif defined(__APPLE__)
#   define N8_BUILD_PLATFORM "darwin"
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   define N8_BUILD_PLATFORM "windows"
#else
#   define N8_BUILD_PLATFORM "unknown"
#endif

#ifndef __EMSCRIPTEN__
#   define N8_BUILD_TYPE        "beta"
#   define N8_VERSION           "v1.0.0-" N8_BUILD_TYPE "-" N8_BUILD_PLATFORM
#   define N8_BUILD_TIME        __TIME__ " " __DATE__
#endif

#define N8_ENV_PATH_NAME "N8_PATH"

#ifdef _WIN32
#   define FS_FILE_SEPARATOR "\\"
#else
#   define FS_FILE_SEPARATOR "/"
#endif

#endif
