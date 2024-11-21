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

#include "n8std/Math.hpp"

#include <n8/ast/TerminativeSignal.hpp>

#include <cmath>
#include <exception>
#include <random>

N8_FUNC(math_cos) {
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

N8_FUNC(math_cosh) {
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

N8_FUNC(math_sin) {
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

N8_FUNC(math_sinh) {
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

N8_FUNC(math_tan) {
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

N8_FUNC(math_tanh) {
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

N8_FUNC(math_acos) {
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

N8_FUNC(math_acosh) {
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

N8_FUNC(math_asin) {
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

N8_FUNC(math_asinh) {
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

N8_FUNC(math_atan) {
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

N8_FUNC(math_atan2) {
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

N8_FUNC(math_atanh) {
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

N8_FUNC(math_rand) {
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

N8_FUNC(math_pow) {
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

N8_FUNC(math_pow2) {
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

N8_FUNC(math_log) {
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

N8_FUNC(math_log10) {
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

N8_FUNC(math_log1p) {
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

N8_FUNC(math_log2) {
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

N8_FUNC(math_exp) {
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

N8_FUNC(math_splitExponent) {
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
    returnValues.emplace_back(DynamicObject(result));
    returnValues.emplace_back(DynamicObject(static_cast<float>(n)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        returnValues
    ));
}

N8_FUNC(math_combineExponent) {
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

N8_FUNC(math_extractExponent) {
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

N8_FUNC(math_scaleByExponent) {
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

N8_FUNC(math_squareRoot) {
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

N8_FUNC(math_cubicRoot) {
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

N8_FUNC(math_inverseSqrt) {
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

N8_FUNC(math_hypotenuse) {
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

N8_FUNC(math_ceil) {
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

N8_FUNC(math_floor) {
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

N8_FUNC(math_round) {
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

N8_FUNC(math_dim) {
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

N8_FUNC(math_min) {
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

N8_FUNC(math_max) {
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

N8_FUNC(math_errorFunc) {
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

N8_FUNC(math_errorFuncComp) {
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

N8_FUNC(math_remainder) {
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

N8_FUNC(math_remQuotient) {
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
    returnValues.emplace_back(DynamicObject(result));
    returnValues.emplace_back(DynamicObject(static_cast<float>(n)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        returnValues
    ));
}

N8_FUNC(math_abs) {
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

N8_FUNC(math_fusedMultiplyAdd) {
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

N8_FUNC(math_sigmoid) {
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

N8_FUNC(math_sigmoidDerivative) {
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

N8_FUNC(math_step) {
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

N8_FUNC(math_relu) {
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

N8_FUNC(math_leakyRelu) {
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

N8_FUNC(math_elu) {
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

N8_FUNC(math_selu) {
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

N8_FUNC(math_softmax) {
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
        return DynamicObject();

    double max = values.at(0).getNumber(), sum = 0.0;
    for(size_t i = 1; i < len; i++) {
        double j = values.at(i).getNumber();

        if(j > max)
            max = j;
    }

    std::vector<double> probabilities;
    for(size_t i = 0; i < len; i++) {
        double temp = exp(values.at(i).getNumber() - max);

        probabilities.emplace_back(temp);
        sum += temp;
    }

    std::vector<DynamicObject> results;
    for(size_t i = 0; i < len; i++)
        results.emplace_back(DynamicObject(probabilities[i] / sum));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        results
    ));
}

N8_FUNC(math_swish) {
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

N8_FUNC(math_mish) {
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

N8_FUNC(math_hardSigmoid) {
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

N8_FUNC(math_hardTan) {
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

N8_FUNC(math_softplus) {
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

N8_FUNC(math_softsign) {
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

N8_FUNC(math_gaussian) {
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

N8_FUNC(math_bentIdentity) {
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

N8_FUNC(math_logLogistic) {
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
