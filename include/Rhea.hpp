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

#ifndef RHEA_HPP
#define RHEA_HPP

#include <rhea/ast/ASTNodeException.hpp>
#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/core/Runtime.hpp>
#include <rhea/core/SymbolTable.hpp>
#include <rhea/parser/LexicalAnalysisException.hpp>
#include <rhea/parser/Parser.hpp>
#include <rhea/parser/ParserException.hpp>
#include <rhea/parser/TokenCategory.hpp>
#include <rhea/util/ArgumentParser.hpp>

#ifdef __TERMUX__
#   define RHEA_BUILD_PLATFORM "termux"
#elif defined(__linux__)
#   define RHEA_BUILD_PLATFORM "linux"
#elif defined(__APPLE__)
#   define RHEA_BUILD_PLATFORM "darwin"
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   define RHEA_BUILD_PLATFORM "windows"
#else
#   define RHEA_BUILD_PLATFORM "unknown"
#endif

#ifndef __EMSCRIPTEN__
#   define RHEA_BUILD_TYPE        "beta"
#   define RHEA_VERSION           "v1.0.0-" RHEA_BUILD_TYPE "-" RHEA_BUILD_PLATFORM
#   define RHEA_BUILD_TIME        __TIME__ " " __DATE__

#   define RHEA_MAIN_BANNER \
    "        \u001b[38;5;87m___\r\n       /   \\\r\n    "       \
    "\u001b[38;5;110m__\u001b[38;5;87m|     |\u001b[0m  "       \
    "\u001b[1;33mRhea Programming Language\u001b[0m\r\n   "     \
    "\u001b[38;5;110m/   \u001b[38;5;87m\\___/\u001b[0m "       \
    "  \u001b[1;33m" RHEA_VERSION "          \u001b[0m\r\n"     \
    "  \u001b[38;5;110m|     |\u001b[0m      \u001b[1;33m"      \
    RHEA_BUILD_TIME "\u001b[0m\r\n   \u001b[38;5;110m\\___"     \
    "/\u001b[0m"

#endif

#define RHEA_ENV_PATH_NAME "RHEA_PATH"

#ifdef _WIN32
#   define FS_FILE_SEPARATOR "\\"
#else
#   define FS_FILE_SEPARATOR "/"
#endif

#define parsync _Pragma("omp parallel for") for

#endif
