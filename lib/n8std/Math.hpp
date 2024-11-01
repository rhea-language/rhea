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

#ifndef N8_STDLIB_MATH_CC
#define N8_STDLIB_MATH_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(math_cos);
N8_FUNC(math_cosh);
N8_FUNC(math_sin);
N8_FUNC(math_sinh);
N8_FUNC(math_tan);
N8_FUNC(math_tanh);

N8_FUNC(math_rand);

N8_FUNC(math_sigmoid);
N8_FUNC(math_sigmoidDerivative);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
