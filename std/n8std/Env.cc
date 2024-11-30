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

#include "n8std/Env.hpp"

#include <n8/ast/TerminativeSignal.hpp>

#include <cstdlib>

N8_FUNC(env_get) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    return DynamicObject(
        std::string(std::getenv(
            value.toString().c_str()
        ))
    );
}

N8_FUNC(env_set) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        value = args.at(1);

    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    return DynamicObject(_putenv(std::string(
        key.toString() + "=" + value.toString()
    ).c_str()) != 0);
    #else
    return DynamicObject(setenv(
        key.toString().c_str(),
        value.toString().c_str(),
        1
    ) != 0);
    #endif
}
