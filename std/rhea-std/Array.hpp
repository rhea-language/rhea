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

#ifndef RHEA_STDLIB_ARRAY_CC
#define RHEA_STDLIB_ARRAY_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(array_create);
RHEA_FUNC(array_clear);

RHEA_FUNC(array_length);
RHEA_FUNC(array_reverse);

RHEA_FUNC(array_first);
RHEA_FUNC(array_last);

RHEA_FUNC(array_add);
RHEA_FUNC(array_pushBack);
RHEA_FUNC(array_pushFront);
RHEA_FUNC(array_assign);
RHEA_FUNC(array_slice);

RHEA_FUNC(array_remove);
RHEA_FUNC(array_removeAt);
RHEA_FUNC(array_removeAll);
RHEA_FUNC(array_removeSlice);

RHEA_FUNC(array_contains);
RHEA_FUNC(array_find);

RHEA_FUNC(array_at);
RHEA_FUNC(array_join);

RHEA_FUNC(array_areAllString);
RHEA_FUNC(array_areAllNumber);
RHEA_FUNC(array_areAllFunction);
RHEA_FUNC(array_areAllBool);
RHEA_FUNC(array_areAllRegex);
RHEA_FUNC(array_areAllArray);
RHEA_FUNC(array_areAllNil);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
