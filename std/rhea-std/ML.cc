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

#include "rhea-std/ML.hpp"

#include <Rhea.hpp>
#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/ast/expression/FunctionDeclarationExpression.hpp>
#include <rhea/util/VectorMath.hpp>

#include <chisei/idx_loader.hpp>

#include <cmath>
#include <exception>
#include <vector>

static inline std::vector<double> arrayToDoubleVector(
    std::shared_ptr<Token> address,
    std::vector<DynamicObject> array
) {
    std::vector<double> values(array.size());

    parsync(size_t i = 0; i < array.size(); i++) {
        if(!array[i].isNumber())
            throw TerminativeThrowSignal(
                std::move(address),
                "Value from array is not a number"
            );

        values[i] = array[i].getNumber();
    }

    return values;
}

static inline double calculateMean(
    std::shared_ptr<Token> address,
    std::vector<DynamicObject> array
) {
    std::vector<double> values = arrayToDoubleVector(
        std::move(address),
        array
    );
    size_t arraySize = array.size();
    double sum = 0.0;

    parsync(size_t i = 0; i < arraySize; i++)
        sum += values[i];

    return sum / arraySize;
}

RHEA_FUNC(ml_trendline_calculate) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject xObj = args.at(0),
        yObj = args.at(1);

    if(!xObj.isArray() || !yObj.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter x and y must be both number array"
        );

    std::vector<DynamicObject> xObjArray = *xObj.getArray();
    std::vector<DynamicObject> yObjArray = *yObj.getArray();

    if(xObjArray.size() != yObjArray.size())
        throw TerminativeThrowSignal(
            std::move(address),
            "Data set size of x and y did not match"
        );

    double x = calculateMean(std::move(address), xObjArray),
        y = calculateMean(std::move(address), yObjArray),
        numerator = 0.0,
        denominator = 0.0;

    parsync(size_t i = 0; i < xObjArray.size(); i++) {
        numerator += (xObjArray[i].getNumber() - x) *
            (yObjArray[i].getNumber() - y);

        denominator += (xObjArray[i].getNumber() - x) *
            (xObjArray[i].getNumber() - x);
    }

    double slope = numerator / denominator;
    std::vector<DynamicObject> returnValues;

    returnValues.push_back(DynamicObject(slope));
    returnValues.push_back(DynamicObject(y - slope * x));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        returnValues
    ));
}

RHEA_FUNC(ml_trendline_calculateRmse) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject xObj = args.at(0),
        yObj = args.at(1),
        model = args.at(2);
    std::vector<DynamicObject> regModel = *model.getArray();

    if(!model.isArray() || regModel.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Invalid linear regression model"
        );

    if(!xObj.isArray() || !yObj.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter x and y must be both number array"
        );

    std::vector<DynamicObject> xObjArray = *xObj.getArray();
    std::vector<DynamicObject> yObjArray = *yObj.getArray();

    if(xObjArray.size() != yObjArray.size())
        throw TerminativeThrowSignal(
            std::move(address),
            "Data set size of x and y did not match"
        );

    double sumSquaredErrs = 0.0;
    size_t paramSize = xObjArray.size();

    for(size_t i = 0; i < paramSize; i++) {
        std::vector<DynamicObject> model;
        model.push_back(regModel.at(0));
        model.push_back(regModel.at(1));

        std::vector<DynamicObject> params;
        params.push_back(DynamicObject(
            std::make_shared<std::vector<DynamicObject>>(model)
        ));
        params.push_back(xObjArray[i]);

        double yPred = ml_trendline_predict(
            std::move(address),
            symtab,
            params,
            unsafe
        ).getNumber();
        double error = yObjArray[i].getNumber() - yPred;

        sumSquaredErrs += error * error;
    }

    return DynamicObject(std::sqrt(sumSquaredErrs / paramSize));
}

RHEA_FUNC(ml_trendline_predict) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject model = args.at(0),
        value = args.at(1);
    std::vector<DynamicObject> regModel = *model.getArray();

    if(!model.isArray() || regModel.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Invalid linear regression model"
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Cannot predict linear regression value for non-numbers"
        );

    DynamicObject slope = regModel.at(0),
        intercept = regModel.at(1);

    if(!slope.isNumber() || !intercept.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Linear regression model's slope and intercept must be a number"
        );

    return DynamicObject(
        slope.getNumber() *
        value.getNumber() +
        intercept.getNumber()
    );
}

RHEA_FUNC(ml_ann_create) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject layers = args.at(0),
        activation = args.at(1),
        activationDerivative = args.at(2);

    if(!layers.isArray() ||
        !RheaUtil::isNumberArray(*layers.getArray().get())
    ) throw TerminativeThrowSignal(
            std::move(address),
            "Layer parameter should be of number array type."
        );

    if(!activation.isFunction() ||
        !activationDerivative.isFunction())
        throw TerminativeThrowSignal(
            std::move(address),
            "Activation function and derivative should be of function type."
        );

    std::string id = RheaUtil::uniqueKey();
    std::vector<double> doubleLayers = RheaUtil::object2Vector(layers);
    std::vector<size_t> layerSizes;

    layerSizes.reserve(doubleLayers.size());
    for(double d : doubleLayers)
        layerSizes.push_back(static_cast<size_t>(d));

    SymbolTable symbols = symtab;
    neuralNetworkMap[id] = std::make_shared<chisei::NeuralNetwork>(
        layerSizes,
        [activation, symbols, unsafe](double arg) -> double {
            std::vector<DynamicObject> callArgs;
            callArgs.emplace_back(DynamicObject(arg));

            DynamicObject result = activation.getCallable()->call(
                symbols,
                callArgs
            );

            return result.isNumber()
                ? result.getNumber()
                : 0.0;
        },
        [activationDerivative, symbols, unsafe](double arg) -> double {
            std::vector<DynamicObject> callArgs;
            callArgs.emplace_back(DynamicObject(arg));

            DynamicObject result = activationDerivative.getCallable()->call(
                symbols,
                callArgs
            );

            return result.isNumber()
                ? result.getNumber()
                : 0.0;
        }
    );

    return DynamicObject(id);
}

RHEA_FUNC(ml_ann_fromMnist) {
    if(args.size() != 4)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 4 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject imageFile = args.at(0),
        labelFile = args.at(1),
        learningRate = args.at(2),
        epoch = args.at(3);

    if(!imageFile.isString() || !labelFile.isString())
        throw TerminativeThrowSignal(
            std::move(address),
            "Image and label file path should be of string type."
        );

    if(!learningRate.isString() || !epoch.isString())
        throw TerminativeThrowSignal(
            std::move(address),
            "Learning rate and epoch should be of number type."
        );

    std::string id = RheaUtil::uniqueKey();
    neuralNetworkMap[id] = std::make_shared<chisei::NeuralNetwork>(
        chisei::IDXLoader::fromMNIST(
            imageFile.toString(),
            labelFile.toString(),
            learningRate.getNumber(),
            epoch.getNumber()
        )
    );

    return DynamicObject(id);
}

RHEA_FUNC(ml_ann_fromModelFile) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject path = args.at(0);
    if(!path.isString())
        throw TerminativeThrowSignal(
            std::move(address),
            "Path parameter should be of string."
        );

    std::string id = RheaUtil::uniqueKey();
    neuralNetworkMap[id] = std::make_unique<chisei::NeuralNetwork>(
        chisei::NeuralNetwork::loadFromModel(path.toString())
    );

    return DynamicObject(id);
}

RHEA_FUNC(ml_ann_train) {
    if(args.size() != 5)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 5 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject ann = args.at(0),
        inputs = args.at(1),
        targets = args.at(2),
        learningRate = args.at(3),
        epoch = args.at(4);

    std::string id = ann.toString();
    if(neuralNetworkMap.count(id) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Neural network map ID not found."
        );

    if(!inputs.getArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Input parameter should be of array of number array type."
        );

    if(!targets.getArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Target parameter should be of array of number of array type."
        );

    if(!learningRate.getNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Learning rate parameter should be of number type."
        );

    if(!epoch.getNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Epoch parameter should be of number type."
        );

    std::vector<std::vector<double>> inputVec, targetVec;
    for(const auto& inpv : *inputs.getArray().get()) {
        if(!inpv.isArray() || !RheaUtil::isNumberArray(*inpv.getArray()))
            throw TerminativeThrowSignal(
                std::move(address),
                "Input parameter elements should be of number array types."
            );

        inputVec.emplace_back(RheaUtil::object2Vector(inpv));
    }

    for(const auto& tgtv : *targets.getArray().get()) {
        if(!tgtv.isArray() || !RheaUtil::isNumberArray(*tgtv.getArray()))
            throw TerminativeThrowSignal(
                std::move(address),
                "Target parameter elements should be of number array types."
            );

        targetVec.emplace_back(RheaUtil::object2Vector(tgtv));
    }

    neuralNetworkMap[id]->train(
        inputVec,
        targetVec,
        learningRate.getNumber(),
        epoch.getNumber()
    );
    return {};
}

RHEA_FUNC(ml_ann_predict) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject ann = args.at(0),
        inputs = args.at(1);

    std::string id = ann.toString();
    if(neuralNetworkMap.count(id) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Neural network map ID not found."
        );

    if(!inputs.isArray() || !RheaUtil::isNumberArray(*inputs.getArray().get()))
        throw TerminativeThrowSignal(
            std::move(address),
            "Inputs parameter should be of number array type."
        );

    return DynamicObject(RheaUtil::vector2Object(
        neuralNetworkMap[id]->predict(
            RheaUtil::object2Vector(inputs)
        )
    ));
}

RHEA_FUNC(ml_ann_calculateMseLoss) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject ann = args.at(0),
        predictions = args.at(1),
        targets = args.at(2);

    std::string id = ann.toString();
    if(neuralNetworkMap.count(id) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Neural network map ID not found."
        );

    if(!predictions.isArray() || !RheaUtil::isNumberArray(*predictions.getArray().get()))
        throw TerminativeThrowSignal(
            std::move(address),
            "Prediction parameter should be of number array type."
        );

    if(!targets.isArray() || !RheaUtil::isNumberArray(*targets.getArray().get()))
        throw TerminativeThrowSignal(
            std::move(address),
            "Targets parameter should be of number array type."
        );

    return DynamicObject(
        neuralNetworkMap[id]->compute_mse_loss(
            RheaUtil::object2Vector(predictions),
            RheaUtil::object2Vector(targets)
        )
    );
}

RHEA_FUNC(ml_ann_computeOutputGradient) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject ann = args.at(0),
        predictions = args.at(1),
        targets = args.at(2);

    std::string id = ann.toString();
    if(neuralNetworkMap.count(id) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Neural network map ID not found."
        );

    if(!predictions.isArray() || !RheaUtil::isNumberArray(*predictions.getArray().get()))
        throw TerminativeThrowSignal(
            std::move(address),
            "Prediction parameter should be of number array type."
        );

    if(!targets.isArray() || !RheaUtil::isNumberArray(*targets.getArray().get()))
        throw TerminativeThrowSignal(
            std::move(address),
            "Targets parameter should be of number array type."
        );

    return DynamicObject(RheaUtil::vector2Object(
        neuralNetworkMap[id]->compute_output_gradient(
            RheaUtil::object2Vector(predictions),
            RheaUtil::object2Vector(targets)
        )
    ));
}

RHEA_FUNC(ml_ann_computeAccuracy) {
    if(args.size() != 5)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 5 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject ann = args.at(0),
        inputs = args.at(1),
        targets = args.at(2);

    std::string id = ann.toString();
    if(neuralNetworkMap.count(id) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Neural network map ID not found."
        );

    if(!inputs.getArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Input parameter should be of array of number array type."
        );

    if(!targets.getArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Target parameter should be of array of number of array type."
        );

    std::vector<std::vector<double>> inputVec, targetVec;
    for(const auto& inpv : *inputs.getArray().get()) {
        if(!inpv.isArray() || !RheaUtil::isNumberArray(*inpv.getArray()))
            throw TerminativeThrowSignal(
                std::move(address),
                "Input parameter elements should be of number array types."
            );

        inputVec.emplace_back(RheaUtil::object2Vector(inpv));
    }

    for(const auto& tgtv : *targets.getArray().get()) {
        if(!tgtv.isArray() || !RheaUtil::isNumberArray(*tgtv.getArray()))
            throw TerminativeThrowSignal(
                std::move(address),
                "Target parameter elements should be of number array types."
            );

        targetVec.emplace_back(RheaUtil::object2Vector(tgtv));
    }

    return DynamicObject(
        neuralNetworkMap[id]->compute_accuracy(
            inputVec,
            targetVec
        )
    );
}

RHEA_FUNC(ml_ann_isCorrectPrediction) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject ann = args.at(0),
        predictions = args.at(1),
        targets = args.at(2);

    std::string id = ann.toString();
    if(neuralNetworkMap.count(id) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Neural network map ID not found."
        );

    if(!predictions.isArray() || !RheaUtil::isNumberArray(*predictions.getArray().get()))
        throw TerminativeThrowSignal(
            std::move(address),
            "Prediction parameter should be of number array type."
        );

    if(!targets.isArray() || !RheaUtil::isNumberArray(*targets.getArray().get()))
        throw TerminativeThrowSignal(
            std::move(address),
            "Targets parameter should be of number array type."
        );

    return DynamicObject(
        neuralNetworkMap[id]->is_correct_prediction(
            RheaUtil::object2Vector(predictions),
            RheaUtil::object2Vector(targets)
        )
    );
}

RHEA_FUNC(ml_ann_saveModel) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject ann = args.at(0),
        path = args.at(1);

    std::string id = ann.toString();
    if(neuralNetworkMap.count(id) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Neural network map ID not found."
        );

    neuralNetworkMap[id]->save_model(path.toString());
    return {};
}
