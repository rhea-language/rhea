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

#ifndef ZHIVO_UTIL_VECTOR_MATH_HPP
#define ZHIVO_UTIL_VECTOR_MATH_HPP

#include <core/DynamicObject.hpp>
#include <vector>

namespace ZhivoUtil {

DynamicObject vector2Object(const std::vector<double>& vec);
std::vector<double> object2Vector(const DynamicObject object);

class VectorMath {
public:
    static std::vector<double> add(
        std::vector<double> left,
        std::vector<double> right
    );
};

};

#endif
