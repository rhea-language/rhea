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

#include "zhvlib/Math.hpp"

#include <cmath>
#include <exception>

ZHIVO_FUNC(math_cos) {
    if(args.size() != 1)
        throw std::runtime_error("Expecting only 1 argument.");

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw std::runtime_error("Argument type is not of number.");

    return DynamicObject(cos(value.getNumber()));
}

ZHIVO_FUNC(math_sin) {
    if(args.size() != 1)
        throw std::runtime_error("Expecting only 1 argument.");

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw std::runtime_error("Argument type is not of number.");

    return DynamicObject(sin(value.getNumber()));
}

ZHIVO_FUNC(math_tan) {
    if(args.size() != 1)
        throw std::runtime_error("Expecting only 1 argument.");

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw std::runtime_error("Argument type is not of number.");

    return DynamicObject(tan(value.getNumber()));
}
