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

#ifndef N8_UTIL_RANDOM_BOOL_HPP
#define N8_UTIL_RANDOM_BOOL_HPP

#include <random>

#if defined(__RDRND__) && defined(__RDSEED__)
#   include <immintrin.h>
#endif

namespace N8Util {

inline static bool randomBoolValue() {
    thread_local std::random_device rd;
    thread_local std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    #if defined(__RDRND__) && defined(__RDSEED__)
    uint32_t rand, seed, genSeed;

    while(_rdrand32_step(&rand) == 0);
    while(_rdseed32_step(&seed) == 0);
    while(_rdrand32_step(&genSeed) == 0);

    gen.seed(genSeed);
    #endif

    return distrib(gen) == 1;
}

};

#endif
