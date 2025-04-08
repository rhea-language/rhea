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

#ifndef RHEA_STDLIB_MATH_CC
#define RHEA_STDLIB_MATH_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(math_cos);
RHEA_FUNC(math_cosh);

RHEA_FUNC(math_sin);
RHEA_FUNC(math_sinh);

RHEA_FUNC(math_tan);
RHEA_FUNC(math_tanh);

RHEA_FUNC(math_acos);
RHEA_FUNC(math_acosh);

RHEA_FUNC(math_asin);
RHEA_FUNC(math_asinh);

RHEA_FUNC(math_atan);
RHEA_FUNC(math_atan2);
RHEA_FUNC(math_atanh);

RHEA_FUNC(math_rand);

RHEA_FUNC(math_pow);
RHEA_FUNC(math_pow2);

RHEA_FUNC(math_log);
RHEA_FUNC(math_log10);
RHEA_FUNC(math_log1p);
RHEA_FUNC(math_log2);

RHEA_FUNC(math_exp);
RHEA_FUNC(math_splitExponent);
RHEA_FUNC(math_combineExponent);
RHEA_FUNC(math_extractExponent);
RHEA_FUNC(math_scaleByExponent);

RHEA_FUNC(math_squareRoot);
RHEA_FUNC(math_cubicRoot);
RHEA_FUNC(math_inverseSqrt);
RHEA_FUNC(math_hypotenuse);

RHEA_FUNC(math_ceil);
RHEA_FUNC(math_floor);
RHEA_FUNC(math_round);

RHEA_FUNC(math_dim);
RHEA_FUNC(math_min);
RHEA_FUNC(math_max);

RHEA_FUNC(math_errorFunc);
RHEA_FUNC(math_errorFuncComp);

RHEA_FUNC(math_remainder);
RHEA_FUNC(math_remQuotient);
RHEA_FUNC(math_abs);
RHEA_FUNC(math_fusedMultiplyAdd);

RHEA_FUNC(math_activation_sigmoid);
RHEA_FUNC(math_activation_sigmoidDerivative);
RHEA_FUNC(math_activation_step);
RHEA_FUNC(math_activation_relu);
RHEA_FUNC(math_activation_leakyRelu);
RHEA_FUNC(math_activation_elu);
RHEA_FUNC(math_activation_selu);
RHEA_FUNC(math_activation_softmax);
RHEA_FUNC(math_activation_swish);
RHEA_FUNC(math_activation_mish);
RHEA_FUNC(math_activation_hardSigmoid);
RHEA_FUNC(math_activation_hardTan);
RHEA_FUNC(math_activation_softplus);
RHEA_FUNC(math_activation_softsign);
RHEA_FUNC(math_activation_gaussian);
RHEA_FUNC(math_activation_bentIdentity);
RHEA_FUNC(math_activation_logLogistic);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
