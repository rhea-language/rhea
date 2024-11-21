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

N8_FUNC(math_acos);
N8_FUNC(math_acosh);

N8_FUNC(math_asin);
N8_FUNC(math_asinh);

N8_FUNC(math_atan);
N8_FUNC(math_atan2);
N8_FUNC(math_atanh);

N8_FUNC(math_rand);

N8_FUNC(math_pow);
N8_FUNC(math_pow2);

N8_FUNC(math_log);
N8_FUNC(math_log10);
N8_FUNC(math_log1p);
N8_FUNC(math_log2);

N8_FUNC(math_exp);
N8_FUNC(math_splitExponent);
N8_FUNC(math_combineExponent);
N8_FUNC(math_extractExponent);
N8_FUNC(math_scaleByExponent);

N8_FUNC(math_squareRoot);
N8_FUNC(math_cubicRoot);
N8_FUNC(math_inverseSqrt);
N8_FUNC(math_hypotenuse);

N8_FUNC(math_ceil);
N8_FUNC(math_floor);
N8_FUNC(math_round);

N8_FUNC(math_dim);
N8_FUNC(math_min);
N8_FUNC(math_max);

N8_FUNC(math_errorFunc);
N8_FUNC(math_errorFuncComp);

N8_FUNC(math_remainder);
N8_FUNC(math_remQuotient);
N8_FUNC(math_abs);
N8_FUNC(math_fusedMultiplyAdd);

N8_FUNC(math_sigmoid);
N8_FUNC(math_sigmoidDerivative);
N8_FUNC(math_step);
N8_FUNC(math_relu);
N8_FUNC(math_leakyRelu);
N8_FUNC(math_elu);
N8_FUNC(math_selu);
N8_FUNC(math_softmax);
N8_FUNC(math_swish);
N8_FUNC(math_mish);
N8_FUNC(math_hardSigmoid);
N8_FUNC(math_hardTan);
N8_FUNC(math_softplus);
N8_FUNC(math_softsign);
N8_FUNC(math_gaussian);
N8_FUNC(math_bentIdentity);
N8_FUNC(math_logLogistic);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
