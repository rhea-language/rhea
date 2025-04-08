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

#include <rhea/util/Convert.hpp>

namespace RheaUtil {

void Convert::reverse(unsigned char* array, size_t length) {
    size_t left = 0, right = length - 1;

    while(left < right) {
        unsigned char temp = array[left];

        array[left] = array[right];
        array[right] = temp;

        left++;
        right--;
    }
}

double Convert::toDouble(const unsigned char* bytes, size_t length) {
    if(bytes == nullptr || length != 8)
        throw std::invalid_argument("Byte array must be non-null and have length 8");

    unsigned char reversedBytes[8];
    std::memcpy(reversedBytes, bytes, 8);
    Convert::reverse(reversedBytes, 8);

    double result;
    std::memcpy(&result, reversedBytes, sizeof(result));

    return result;
}

std::vector<unsigned char> Convert::toBytes(double number) {
    std::vector<unsigned char> bytes(8);
    std::memcpy(bytes.data(), &number, sizeof(number));

    Convert::reverse(bytes.data(), 8);
    return bytes;
}

double Convert::translateDigit(const std::string& image) {
    if(image.empty())
        throw std::invalid_argument("Input string is null or empty");

    if(image.find("0b") == 0)
        return Convert::parseBinary(image.substr(2));
    else if(image.find("0t") == 0)
        return Convert::parseBase3(image.substr(2));
    else if(image.find("0c") == 0)
        return Convert::parseOctal(image.substr(2));
    else if(image.find("0x") == 0)
        return Convert::parseHex(image.substr(2));

    return std::stod(image);
}

double Convert::parseBinary(const std::string& str) {
    return static_cast<double>(std::stoi(str, nullptr, 2));
}

double Convert::parseBase3(const std::string& str) {
    return static_cast<double>(std::stoi(str, nullptr, 3));
}

double Convert::parseOctal(const std::string& str) {
    return static_cast<double>(std::stoi(str, nullptr, 8));
}

double Convert::parseHex(const std::string& str) {
    return static_cast<double>(std::stoi(str, nullptr, 16));
}

};
