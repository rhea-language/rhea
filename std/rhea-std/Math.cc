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

#include "rhea-std/Math.hpp"

#include <Rhea.hpp>
#include <rhea/ast/TerminativeSignal.hpp>

#include <cmath>
#include <exception>
#include <random>

RHEA_FUNC(math_cos) {
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

RHEA_FUNC(math_cosh) {
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

RHEA_FUNC(math_sin) {
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

RHEA_FUNC(math_sinh) {
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

RHEA_FUNC(math_tan) {
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

RHEA_FUNC(math_tanh) {
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

RHEA_FUNC(math_acos) {
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

    return DynamicObject(acos(value.getNumber()));
}

RHEA_FUNC(math_acosh) {
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

    return DynamicObject(acosh(value.getNumber()));
}

RHEA_FUNC(math_asin) {
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

    return DynamicObject(asin(value.getNumber()));
}

RHEA_FUNC(math_asinh) {
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

    return DynamicObject(asinh(value.getNumber()));
}

RHEA_FUNC(math_atan) {
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

    return DynamicObject(atan(value.getNumber()));
}

RHEA_FUNC(math_atan2) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject y = args.at(0),
        x = args.at(1);
    if(!y.isNumber() || !x.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(atan2(y.getNumber(), x.getNumber()));
}

RHEA_FUNC(math_atanh) {
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

    return DynamicObject(atanh(value.getNumber()));
}

RHEA_FUNC(math_rand) {
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

RHEA_FUNC(math_pow) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(pow(x.getNumber(), y.getNumber()));
}

RHEA_FUNC(math_pow2) {
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

    return DynamicObject(exp2(value.getNumber()));
}

RHEA_FUNC(math_log) {
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

    return DynamicObject(log(value.getNumber()));
}

RHEA_FUNC(math_log10) {
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

    return DynamicObject(log10(value.getNumber()));
}

RHEA_FUNC(math_log1p) {
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

    return DynamicObject(log1p(value.getNumber()));
}

RHEA_FUNC(math_log2) {
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

    return DynamicObject(log2(value.getNumber()));
}

RHEA_FUNC(math_exp) {
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

    return DynamicObject(exp(value.getNumber()));
}

RHEA_FUNC(math_splitExponent) {
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

    int n;
    double param = value.getNumber(),
        result = frexp(param, &n);

    std::vector<DynamicObject> returnValues;
    returnValues.push_back(DynamicObject(result));
    returnValues.push_back(DynamicObject(static_cast<float>(n)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        returnValues
    ));
}

RHEA_FUNC(math_combineExponent) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(ldexp(
        x.getNumber(),
        static_cast<int>(y.getNumber())
    ));
}

RHEA_FUNC(math_extractExponent) {
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

    return DynamicObject(logb(value.getNumber()));
}

RHEA_FUNC(math_scaleByExponent) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(scalbn(
        x.getNumber(),
        static_cast<int>(y.getNumber())
    ));
}

RHEA_FUNC(math_squareRoot) {
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

    return DynamicObject(sqrt(value.getNumber()));
}

RHEA_FUNC(math_cubicRoot) {
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

    return DynamicObject(cbrt(value.getNumber()));
}

RHEA_FUNC(math_inverseSqrt) {
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

    // Based on the Quake III Fast Inversed Square Root Algorithm
    union {
        float f;
        uint32_t i;
    } conv;

    float x2, number = value.getNumber();
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    conv.f  = number;
    conv.i  = 0x5f3759df - (conv.i >> 1);
    conv.f  = conv.f * (threehalfs - (x2 * conv.f * conv.f));

    return DynamicObject(conv.f);
}

RHEA_FUNC(math_hypotenuse) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(hypot(
        x.getNumber(),
        y.getNumber()
    ));
}

RHEA_FUNC(math_ceil) {
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

    return DynamicObject(ceil(value.getNumber()));
}

RHEA_FUNC(math_floor) {
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

    return DynamicObject(floor(value.getNumber()));
}

RHEA_FUNC(math_round) {
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

    return DynamicObject(round(value.getNumber()));
}

RHEA_FUNC(math_dim) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(fdim(
        x.getNumber(),
        y.getNumber()
    ));
}

RHEA_FUNC(math_min) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(fmin(
        x.getNumber(),
        y.getNumber()
    ));
}

RHEA_FUNC(math_max) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(fmax(
        x.getNumber(),
        y.getNumber()
    ));
}

RHEA_FUNC(math_errorFunc) {
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

    return DynamicObject(erf(value.getNumber()));
}

RHEA_FUNC(math_errorFuncComp) {
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

    return DynamicObject(erfc(value.getNumber()));
}

RHEA_FUNC(math_remainder) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(remainder(
        x.getNumber(),
        y.getNumber()
    ));
}

RHEA_FUNC(math_remQuotient) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    int n;
    double xn = x.getNumber(),
        yn = y.getNumber(),
        result = remquo(xn, yn, &n);

    std::vector<DynamicObject> returnValues;
    returnValues.push_back(DynamicObject(result));
    returnValues.push_back(DynamicObject(static_cast<float>(n)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        returnValues
    ));
}

RHEA_FUNC(math_abs) {
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

    return DynamicObject(fabs(value.getNumber()));
}

RHEA_FUNC(math_fusedMultiplyAdd) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1),
        z = args.at(2);
    if(!x.isNumber() || !y.isNumber() || !z.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    return DynamicObject(fma(
        x.getNumber(),
        y.getNumber(),
        z.getNumber()
    ));
}

RHEA_FUNC(math_activation_sigmoid) {
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

RHEA_FUNC(math_activation_sigmoidDerivative) {
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

RHEA_FUNC(math_activation_step) {
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
    return DynamicObject(num >= 0 ? 1.0 : 0.0);
}

RHEA_FUNC(math_activation_relu) {
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
    return DynamicObject(num > 0 ? num : 0);
}

RHEA_FUNC(math_activation_leakyRelu) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    double xn = x.getNumber();
    return DynamicObject(xn > 0 ? xn : y.getNumber() * xn);
}

RHEA_FUNC(math_activation_elu) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "One of the argument type is not of number."
        );

    double xn = x.getNumber();
    return DynamicObject(xn > 0 ? xn :
        y.getNumber() * (exp(xn) - 1)
    );
}

RHEA_FUNC(math_activation_selu) {
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

    const double lambda = 1.0507;
    const double alpha = 1.67326;
    double num = value.getNumber();

    return DynamicObject(
        num > 0 ?
            lambda * num :
            lambda * alpha * (exp(num) - 1)
    );
}

RHEA_FUNC(math_activation_softmax) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Argument type is not of array."
        );

    std::vector<DynamicObject> values = *value.getArray();
    size_t len = values.size();
    if(len == 0)
        return {};

    double max = values.at(0).getNumber(), sum = 0.0;
    parsync(size_t i = 1; i < len; i++) {
        double j = values.at(i).getNumber();

        if(j > max)
            max = j;
    }

    std::vector<double> probabilities;
    for(size_t i = 0; i < len; i++) {
        double temp = exp(values.at(i).getNumber() - max);

        probabilities.push_back(temp);
        sum += temp;
    }

    std::vector<DynamicObject> results;
    for(size_t i = 0; i < len; i++)
        results.push_back(DynamicObject(probabilities[i] / sum));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        results
    ));
}

RHEA_FUNC(math_activation_swish) {
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
    return DynamicObject(num / (1.0 + exp(-num)));
}

RHEA_FUNC(math_activation_mish) {
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
    return DynamicObject(num * tanh(log1p(exp(num))));
}

RHEA_FUNC(math_activation_hardSigmoid) {
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

    return DynamicObject(
        fmax(0.0, fmin(1.0, fma(0.2, value.getNumber(), 0.5)))
    );
}

RHEA_FUNC(math_activation_hardTan) {
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

    return DynamicObject(
        fmax(-1.0, fmin(1.0, value.getNumber()))
    );
}

RHEA_FUNC(math_activation_softplus) {
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

    return DynamicObject(
        log1p(exp(value.getNumber()))
    );
}

RHEA_FUNC(math_activation_softsign) {
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
    return DynamicObject(num / (1.0 + fabs(num)));
}

RHEA_FUNC(math_activation_gaussian) {
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
    return DynamicObject(exp(-num * num));
}

RHEA_FUNC(math_activation_bentIdentity) {
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
    return DynamicObject((sqrt(fma(num, num, 1.0) - 1.0) / 2.0) + num);
}

RHEA_FUNC(math_activation_logLogistic) {
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
    return DynamicObject(1.0 / (1.0 + exp(-num)));
}
