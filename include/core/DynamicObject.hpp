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

#ifndef ZHIVO_DYNAMIC_OBJECT_HPP
#define ZHIVO_DYNAMIC_OBJECT_HPP

#include <core/DynamicObjectType.hpp>
#include <core/RegexWrapper.hpp>
#include <parser/Token.hpp>

#include <functional>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>

class DynamicObject;
class SymbolTable;
class FunctionDeclarationExpression;

class DynamicObject {
private:
    DynamicObjectType type;

    std::shared_ptr<FunctionDeclarationExpression> functionValue;
    std::shared_ptr<std::vector<DynamicObject>> arrayValue;
    std::shared_ptr<RegexWrapper> regexValue;
    std::string stringValue;
    double numberValue;
    bool boolValue;

public:
    DynamicObject(std::shared_ptr<FunctionDeclarationExpression> value) :
        type(DynamicObjectType::FUNCTION),
        functionValue(std::move(value)),
        arrayValue(nullptr),
        regexValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(std::shared_ptr<RegexWrapper> value) :
        type(DynamicObjectType::REGEX),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(std::move(value)),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(std::shared_ptr<std::vector<DynamicObject>> value) :
        type(DynamicObjectType::ARRAY),
        functionValue(nullptr),
        arrayValue(std::move(value)),
        regexValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(std::string value) :
        type(DynamicObjectType::STRING),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        stringValue(std::move(value)),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(double value) :
        type(DynamicObjectType::NUMBER),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        stringValue(std::move("")),
        numberValue(value),
        boolValue(false) {}

    DynamicObject(bool value) :
        type(DynamicObjectType::BOOL),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(value) {}

    DynamicObject() :
        type(DynamicObjectType::NIL),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(const DynamicObject& other) :
        type(other.type),
        functionValue(other.functionValue),
        arrayValue(other.arrayValue),
        regexValue(other.regexValue),
        stringValue(other.stringValue),
        numberValue(other.numberValue),
        boolValue(other.boolValue) {}

    DynamicObject& operator=(const DynamicObject& other);
    DynamicObject& operator=(DynamicObject&& other);
    bool operator==(const DynamicObject& other);
    bool operator!=(const DynamicObject& other);
    bool booleanEquivalent();

    bool isFunction() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;
    bool isRegex() const;
    bool isBool() const;
    bool isNil() const;

    std::shared_ptr<FunctionDeclarationExpression> getCallable() const;
    std::shared_ptr<std::vector<DynamicObject>> getArray() const;
    std::shared_ptr<RegexWrapper> getRegex() const;
    const std::string& getString() const;
    double getNumber() const;
    bool getBool() const;

    void setArrayElement(
        std::unique_ptr<Token> reference,
        size_t index,
        std::unique_ptr<DynamicObject> object
    );

    std::string objectType();
    std::string toString();
};

#endif
