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

#ifndef N8_UTIL_CONVERT_HPP
#define N8_UTIL_CONVERT_HPP

#include <algorithm>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace N8Util {

class Convert final {
public:
    static double toDouble(const unsigned char* bytes, size_t length);
    static std::vector<unsigned char> toBytes(double number);

    static double translateDigit(const std::string& image);

private:
    static void reverse(unsigned char* array, size_t length);

    static double parseBinary(const std::string& str);
    static double parseBase3(const std::string& str);
    static double parseOctal(const std::string& str);
    static double parseHex(const std::string& str);
};

};

#endif
