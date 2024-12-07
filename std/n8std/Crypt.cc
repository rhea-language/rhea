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

#include "n8std/Crypt.hpp"

#include <n8/ast/TerminativeSignal.hpp>

#include <quickdigest5.hpp>
#include <regex>

N8_FUNC(crypt_md5) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    return DynamicObject(
        QuickDigest5::toHash(input.toString())
    );
}

N8_FUNC(crypt_validateMd5) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string md5 = input.toString();
    return DynamicObject(
        std::regex_match(
            md5,
            std::regex("^[a-fA-F0-9]{32}$")
        ) && md5.length() == 32
    );
}
