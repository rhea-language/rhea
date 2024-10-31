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

#ifndef ZHIVO_STDLIB_MATH_CC
#define ZHIVO_STDLIB_MATH_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "ZhivoLibrary.hpp"

ZHIVO_LIB_START

ZHIVO_FUNC(math_cos);
ZHIVO_FUNC(math_cosh);
ZHIVO_FUNC(math_sin);
ZHIVO_FUNC(math_sinh);
ZHIVO_FUNC(math_tan);
ZHIVO_FUNC(math_tanh);

ZHIVO_FUNC(math_rand);

ZHIVO_FUNC(math_sigmoid);
ZHIVO_FUNC(math_sigmoidDerivative);

ZHIVO_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
