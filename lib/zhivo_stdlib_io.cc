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

#include "zhivo_stdlib_io.hpp"

ZHIVO_FUNC(io_print) {
    if(args.size() == 0)
        return {};

    for(size_t i = 0; i < args.size(); i++) {
        DynamicObject arg = args.at(i);
        std::cout << arg.toString();
    }

    return {};
}

ZHIVO_FUNC(io_exit) {
    if(args.size() == 0)
        exit(0);

    DynamicObject exitCode = args.at(0);
    if(!exitCode.isNumber())
        throw std::runtime_error("Exit code is not a number.");

    exit(static_cast<int>(exitCode.getNumber()));
    return {};
}
