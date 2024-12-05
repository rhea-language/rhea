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

#ifndef N8_STDLIB_ARRAY_CC
#define N8_STDLIB_ARRAY_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(array_create);
N8_FUNC(array_clear);

N8_FUNC(array_length);
N8_FUNC(array_reverse);

N8_FUNC(array_first);
N8_FUNC(array_last);

N8_FUNC(array_add);
N8_FUNC(array_pushBack);
N8_FUNC(array_pushFront);
N8_FUNC(array_assign);
N8_FUNC(array_slice);

N8_FUNC(array_remove);
N8_FUNC(array_removeAt);
N8_FUNC(array_removeAll);
N8_FUNC(array_removeSlice);

N8_FUNC(array_contains);
N8_FUNC(array_find);

N8_FUNC(array_at);
N8_FUNC(array_join);

N8_FUNC(array_areAllString);
N8_FUNC(array_areAllNumber);
N8_FUNC(array_areAllFunction);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
