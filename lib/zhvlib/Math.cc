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

#include <zhivo/ast/TerminativeSignal.hpp>

#include <cmath>
#include <exception>
#include <random>

ZHIVO_FUNC(math_cos) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    return DynamicObject(cos(value.getNumber()));
}

ZHIVO_FUNC(math_cosh) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    return DynamicObject(cosh(value.getNumber()));
}

ZHIVO_FUNC(math_sin) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    return DynamicObject(sin(value.getNumber()));
}

ZHIVO_FUNC(math_sinh) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    return DynamicObject(sinh(value.getNumber()));
}

ZHIVO_FUNC(math_tan) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    return DynamicObject(tan(value.getNumber()));
}

ZHIVO_FUNC(math_tanh) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    return DynamicObject(tanh(value.getNumber()));
}

ZHIVO_FUNC(math_rand) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double value = 0.0f;

    if(args.size() == 1) {
        DynamicObject arg = args.at(0);
        if(!arg.isNumber())
            throw TerminativeThrowSignal(
                std::move(address),
                "Expecting a number argument."
            );

        std::mt19937 gen(arg.getNumber());
        value = dis(gen);
    }
    else {
        std::random_device rd;
        std::mt19937 gen(rd());

        value = dis(gen);
    }

    return DynamicObject(value);
}

ZHIVO_FUNC(math_sigmoid) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    return DynamicObject(1 / (1 + exp(-value.getNumber())));
}

ZHIVO_FUNC(math_sigmoidDerivative) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of number."
        );

    double num = value.getNumber();
    return DynamicObject(num * (1 - num));
}
