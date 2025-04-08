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

#ifndef RHEA_UTIL_VECTOR_MATH_HPP
#define RHEA_UTIL_VECTOR_MATH_HPP

#include <rhea/core/DynamicObject.hpp>
#include <vector>

namespace RheaUtil {

bool isNumberArray(std::vector<DynamicObject> vec);
DynamicObject vector2Object(const std::vector<double>& vec);
std::vector<double> object2Vector(const DynamicObject object);

class VectorMath final {
public:
    static std::vector<double> add(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> addSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> sub(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> subSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> div(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> divSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> mul(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> mulSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> rem(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> remSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> bitwiseAnd(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> bitwiseAndSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> bitwiseOr(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> bitwiseOrSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> bitwiseXor(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> bitwiseXorSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> shiftLeftSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> shiftLeft(
        std::vector<double> left,
        std::vector<double> right
    );

    static std::vector<double> shiftRightSingle(
        double value,
        std::vector<double> array
    );

    static std::vector<double> shiftRight(
        std::vector<double> left,
        std::vector<double> right
    );
};

};

#endif
