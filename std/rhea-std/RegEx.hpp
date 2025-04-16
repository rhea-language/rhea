/*
 * Copyright (c) 2025 - Nathanne Isip
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

#ifndef RHEA_STDLIB_REGEX_CC
#define RHEA_STDLIB_REGEX_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(regex_match);
RHEA_FUNC(regex_isValidExpr);
RHEA_FUNC(regex_escapeRegex);

RHEA_FUNC(regex_search);
RHEA_FUNC(regex_countMatches);

RHEA_FUNC(regex_findMatchPositions);
RHEA_FUNC(regex_findAllMatches);

RHEA_FUNC(regex_replaceAll);
RHEA_FUNC(regex_replaceFirst);

RHEA_FUNC(regex_split);
RHEA_FUNC(regex_getCapturedGroups);
RHEA_FUNC(regex_getAllCapturedGroups);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
