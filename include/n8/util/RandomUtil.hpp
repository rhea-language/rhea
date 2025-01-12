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

#ifndef N8_UTIL_RANDOM_UTIL_HPP
#define N8_UTIL_RANDOM_UTIL_HPP

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

inline static std::string uniqueKey() {
    const std::string characters =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789";
    std::string randomString;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(
        static_cast<uint32_t>(0),
        static_cast<uint32_t>(characters.size() - 1)
    );

    #if defined(__RDRND__) && defined(__RDSEED__)
    uint32_t rand, seed, genSeed;

    while(_rdrand32_step(&rand) == 0);
    while(_rdseed32_step(&seed) == 0);
    while(_rdrand32_step(&genSeed) == 0);

    gen.seed(genSeed);
    #endif

    #pragma omp parallel for
    for(size_t i = 0; i < 8; ++i)
        randomString += characters[
            static_cast<uint32_t>(dis(gen))
        ];

    return randomString;
}

};

#endif
