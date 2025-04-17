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

#include "rhea-std/Conv.hpp"

#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/util/Convert.hpp>
 
RHEA_FUNC(conv_toString) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    return DynamicObject(args.at(0).toString());
}

RHEA_FUNC(conv_toNumber) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    return DynamicObject(
        RheaUtil::Convert::translateDigit(
            args.at(0).toString()
        )
    );
}

RHEA_FUNC(conv_toRegex) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    return DynamicObject(
        std::make_shared<RegexWrapper>(
            args.at(0).toString()
        )
    );
}

RHEA_FUNC(conv_toBool) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject obj = args.at(0);
    if(obj.isString()) {
        std::string objStr = obj.toString();
        return DynamicObject(
            objStr == "true" ?
                true : objStr == "false" ?
                    false : obj.booleanEquivalent()
        );
    }

    return DynamicObject(obj.booleanEquivalent());
}
