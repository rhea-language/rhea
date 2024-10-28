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

#include "zhvlib/ML.hpp"

#include <cmath>
#include <exception>
#include <vector>

static inline std::vector<double> arrayToDoubleVector(std::vector<DynamicObject> array) {
    std::vector<double> values(array.size());
    
    #pragma omp parallel for
    for(size_t i = 0; i < array.size(); i++) {
        if(!array[i].isNumber())
            throw std::runtime_error("Value from array is not a number");

        values[i] = array[i].getNumber();
    }

    return values;
}

static inline double calculateMean(std::vector<DynamicObject> array) {
    std::vector<double> values = arrayToDoubleVector(array);
    size_t arraySize = array.size();
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for(size_t i = 0; i < arraySize; i++)
        sum += values[i];

    return sum / arraySize;
}

ZHIVO_FUNC(ml_trendline_calculate) {
    if(args.size() != 2)
        throw std::runtime_error(
            "Expecting 2 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject xObj = args.at(0),
        yObj = args.at(1);

    if(!xObj.isArray() || !yObj.isArray())
        throw std::runtime_error("Parameter x and y must be both number array");

    std::vector<DynamicObject> xObjArray = *xObj.getArray();
    std::vector<DynamicObject> yObjArray = *yObj.getArray();

    if(xObjArray.size() != yObjArray.size())
        throw std::runtime_error("Data set size of x and y did not match");

    double x = calculateMean(xObjArray),
        y = calculateMean(yObjArray),
        numerator = 0.0,
        denominator = 0.0;

    #pragma omp parallel for
    for(size_t i = 0; i < xObjArray.size(); i++) {
        numerator += (xObjArray[i].getNumber() - x) *
            (yObjArray[i].getNumber() - y);

        denominator += (xObjArray[i].getNumber() - x) *
            (xObjArray[i].getNumber() - x);
    }

    double slope = numerator / denominator;
    std::vector<DynamicObject> returnValues;

    returnValues.emplace_back(DynamicObject(slope));
    returnValues.emplace_back(DynamicObject(y - slope * x));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        returnValues
    ));
}

ZHIVO_FUNC(ml_trendline_calculateRmse) {
    if(args.size() != 3)
        throw std::runtime_error(
            "Expecting 3 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject xObj = args.at(0),
        yObj = args.at(1),
        model = args.at(2);
    std::vector<DynamicObject> regModel = *model.getArray();

    if(!model.isArray() || regModel.size() != 2)
        throw std::runtime_error("Invalid linear regression model");

    if(!xObj.isArray() || !yObj.isArray())
        throw std::runtime_error("Parameter x and y must be both number array");

    std::vector<DynamicObject> xObjArray = *xObj.getArray();
    std::vector<DynamicObject> yObjArray = *yObj.getArray();

    if(xObjArray.size() != yObjArray.size())
        throw std::runtime_error("Data set size of x and y did not match");

    double sumSquaredErrs = 0.0;
    size_t paramSize = xObjArray.size();

    #pragma omp parallel for
    for(size_t i = 0; i < paramSize; i++) {
        std::vector<DynamicObject> model;
        model.emplace_back(regModel.at(0));
        model.emplace_back(regModel.at(1));

        std::vector<DynamicObject> params;
        params.emplace_back(DynamicObject(
            std::make_shared<std::vector<DynamicObject>>(model)
        ));
        params.emplace_back(xObjArray[i]);

        double yPred = ml_trendline_predict(symtab, params)
            .getNumber();
        double error = yObjArray[i].getNumber() - yPred;

        sumSquaredErrs += error * error;
    }

    return DynamicObject(std::sqrt(sumSquaredErrs / paramSize));
}

ZHIVO_FUNC(ml_trendline_predict) {
    if(args.size() != 2)
        throw std::runtime_error(
            "Expecting 3 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject model = args.at(0),
        value = args.at(1);
    std::vector<DynamicObject> regModel = *model.getArray();

    if(!model.isArray() || regModel.size() != 2)
        throw std::runtime_error("Invalid linear regression model");

    if(!value.isNumber())
        throw std::runtime_error("Cannot predict linear regression value for non-numbers");

    DynamicObject slope = regModel.at(0),
        intercept = regModel.at(1);

    if(!slope.isNumber() || !intercept.isNumber())
        throw std::runtime_error("Linear regression model's slope and intercept must be a number");

    return DynamicObject(
        slope.getNumber() *
        value.getNumber() +
        intercept.getNumber()
    );
}
