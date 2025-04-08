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

#ifndef RHEA_STDLIB_STR_CC
#define RHEA_STDLIB_STR_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(str_append);
RHEA_FUNC(str_at);
RHEA_FUNC(str_contains);
RHEA_FUNC(str_find);
RHEA_FUNC(str_fromBuffer);
RHEA_FUNC(str_occurence);
RHEA_FUNC(str_pop);
RHEA_FUNC(str_replace);
RHEA_FUNC(str_replaceAll);
RHEA_FUNC(str_split);
RHEA_FUNC(str_substring);
RHEA_FUNC(str_toArray);
RHEA_FUNC(str_toBytes);
RHEA_FUNC(str_trim);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
