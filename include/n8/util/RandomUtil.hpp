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

#include <array>
#include <iomanip>
#include <random>
#include <sstream>

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

inline static std::string generateUuid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

    #if defined(__RDRND__) && defined(__RDSEED__)
    uint32_t rand, seed, genSeed;

    while(_rdrand32_step(&rand) == 0);
    while(_rdseed32_step(&seed) == 0);
    while(_rdrand32_step(&genSeed) == 0);

    gen.seed(genSeed);
    #endif

    std::array<uint8_t, 16> uuid;
    for(size_t i = 0; i < 16; i += 4) {
        uint32_t randomValue = dis(gen);

        uuid[i] = static_cast<uint8_t>((randomValue >> 24) & 0xFF);
        uuid[i + 1] = static_cast<uint8_t>((randomValue >> 16) & 0xFF);
        uuid[i + 2] = static_cast<uint8_t>((randomValue >> 8) & 0xFF);
        uuid[i + 3] = static_cast<uint8_t>(randomValue & 0xFF);
    }

    uuid[6] = (uuid[6] & 0x0F) | 0x40;
    uuid[8] = (uuid[8] & 0x3F) | 0x80;

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for(size_t i = 0; i < 16; ++i) {
        oss << std::setw(2) << static_cast<int>(uuid[i]);

        if(i == 3 || i == 5 || i == 7 || i == 9)
            oss << '-';
    }

    return oss.str();
}

};

#endif
