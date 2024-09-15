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

#include <ast/expression/FunctionDeclarationExpression.hpp>
#include <core/DynamicObject.hpp>
#include <core/RegexWrapper.hpp>

#include <cmath>

DynamicObject& DynamicObject::operator=(const DynamicObject& other) {
    if(this != &other) {
        this->type = other.type;
        this->numberValue = other.numberValue;
        this->boolValue = other.boolValue;
        this->stringValue = other.stringValue; 

        this->arrayValue.reset(other.arrayValue.get());
        this->functionValue.reset(other.functionValue.get());
        this->regexValue.reset(other.regexValue.get());
    }

    return *this;
}

DynamicObject& DynamicObject::operator=(DynamicObject&& other) {
    if(this != &other) {
        this->type = std::move(other.type);
        this->arrayValue = std::move(other.arrayValue);
        this->numberValue = std::move(other.numberValue);
        this->stringValue = std::move(other.stringValue);
        this->boolValue = std::move(other.boolValue);
        this->functionValue = std::move(other.functionValue);
        this->regexValue = std::move(other.regexValue);
    }

    return *this;
}

bool DynamicObject::operator==(const DynamicObject& other) {
    if(this->isNil() && other.isNil())
        return true;
    else if(this->isBool() && other.isBool())
        return this->getBool() == other.getBool();
    else if(this->isNumber() && other.isNumber())
        return std::fabs(this->getNumber() - other.getNumber()) < __DBL_EPSILON__;
    else if(this->isString() && other.isString())
        return this->getString() == other.getString();
    else if(this->isRegex() && other.isRegex())
        return this->getRegex()->getPattern() == other.getRegex()->getPattern();
    else if(this->isFunction() && other.isFunction())
        return this->getCallable()->getFunctionImage() ==
            other.getCallable()->getFunctionImage();
    else if(this->isArray() && other.isArray()) {
        size_t len = this->getArray()->size();
        if(len != other.getArray()->size())
            return false;

        auto left = this->getArray(),
            right = other.getArray();
        for(size_t i = 0; i < len; i++)
            if(!(left->at(i) == right->at(i)))
                return false;

        return true;
    }

    return false;
}

bool DynamicObject::isNumber() const {
    return this->type == DynamicObjectType::NUMBER;
}

bool DynamicObject::isString() const {
    return this->type == DynamicObjectType::STRING;
}

bool DynamicObject::isBool() const {
    return this->type == DynamicObjectType::BOOL;
}

bool DynamicObject::isArray() const {
    return this->type == DynamicObjectType::ARRAY;
}

bool DynamicObject::isFunction() const {
    return this->type == DynamicObjectType::FUNCTION;
}

bool DynamicObject::isNil() const {
    return this->type == DynamicObjectType::NIL;
}

bool DynamicObject::isRegex() const {
    return this->type == DynamicObjectType::REGEX;
}

double DynamicObject::getNumber() const {
    return this->numberValue;
}

const std::string& DynamicObject::getString() const {
    return this->stringValue;
}

bool DynamicObject::getBool() const {
    return this->boolValue;
}

std::shared_ptr<FunctionDeclarationExpression> DynamicObject::getCallable() const {
    return this->functionValue;
}

std::shared_ptr<RegexWrapper> DynamicObject::getRegex() const {
    return this->regexValue;
}

std::shared_ptr<std::vector<DynamicObject>> DynamicObject::getArray() const {
    return this->arrayValue;
}

bool DynamicObject::booleanEquivalent() {
    return (this->isBool() && this->getBool()) ||
        (this->isNumber() && this->getNumber() < 0.0) ||
        (this->isString() && !this->getString().empty()) ||
        (this->isArray() && this->getArray()->size()) ||
        this->isFunction() || this->isRegex();
}

void DynamicObject::setArrayElement(size_t index, std::unique_ptr<DynamicObject> object) {
    if(!this->isArray())
        throw std::runtime_error("Object not an array.");

    if(index >= this->arrayValue->size())
        throw std::out_of_range("Index is out of bounds.");

    (*this->arrayValue)[index] = std::move(*object);
}

std::string DynamicObject::toString() {
    if(this->isNil())
        return "nil";
    else if(this->isNumber())
        return std::to_string(this->getNumber());
    else if(this->isString())
        return this->getString();
    else if(this->isBool())
        return (this->getBool() ? "true" : "false");
    else if(this->isRegex())
        return this->regexValue->getPattern();
    else if(this->isFunction()) {
        const auto& functionImage = this->getCallable()->getFunctionImage();
        return "<func [" + std::to_string(functionImage.getLine()) +
            ", " + std::to_string(functionImage.getColumn()) +
            "]: " + functionImage.getFileName() +
            ">";
    }
    else if(this->isArray()) {
        std::shared_ptr<std::vector<DynamicObject>> array = this->getArray();
        std::string result = "[";

        for(size_t i = 0; i < array->size(); i++) {
            result += array->at(i).toString();

            if(i < array->size() - 1)
                result += ", ";
        }

        result += "]";
        return result;
    }

    throw std::runtime_error("Unknown dynamic object data.");
}
