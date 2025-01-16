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

#ifndef N8_DYNAMIC_OBJECT_HPP
#define N8_DYNAMIC_OBJECT_HPP

#include <n8/core/DynamicObjectType.hpp>
#include <n8/core/RegexWrapper.hpp>
#include <n8/parser/Token.hpp>

#include <cstring>
#include <functional>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>

class DynamicObject;
class SymbolTable;
class FunctionDeclarationExpression;

using NativeFunction = DynamicObject(
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    __stdcall
    #endif
*)(
    std::shared_ptr<Token>,
    SymbolTable&,
    std::vector<DynamicObject>&
);

class DynamicObject final {
private:
    DynamicObjectType type;
    bool isLocked;
    std::string owner;

    std::shared_ptr<FunctionDeclarationExpression> functionValue;
    std::shared_ptr<std::vector<DynamicObject>> arrayValue;
    std::shared_ptr<RegexWrapper> regexValue;
    NativeFunction nativeValue;
    std::string stringValue;
    double numberValue;
    bool boolValue;

public:
    DynamicObject(std::shared_ptr<FunctionDeclarationExpression> value) :
        type(DynamicObjectType::FUNCTION),
        isLocked(false),
        owner(""),
        functionValue(std::move(value)),
        arrayValue(nullptr),
        regexValue(nullptr),
        nativeValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(std::shared_ptr<RegexWrapper> value) :
        type(DynamicObjectType::REGEX),
        isLocked(false),
        owner(""),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(std::move(value)),
        nativeValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(std::shared_ptr<std::vector<DynamicObject>> value) :
        type(DynamicObjectType::ARRAY),
        isLocked(false),
        owner(""),
        functionValue(nullptr),
        arrayValue(std::move(value)),
        regexValue(nullptr),
        nativeValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(std::string value) :
        type(DynamicObjectType::STRING),
        isLocked(false),
        owner(""),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        nativeValue(nullptr),
        stringValue(std::move(value)),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(double value) :
        type(DynamicObjectType::NUMBER),
        isLocked(false),
        owner(""),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        nativeValue(nullptr),
        stringValue(std::move("")),
        numberValue(value),
        boolValue(false) {}

    DynamicObject(bool value) :
        type(DynamicObjectType::BOOL),
        isLocked(false),
        owner(""),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        nativeValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(value) {}

    DynamicObject(NativeFunction value) :
        type(DynamicObjectType::NATIVE),
        isLocked(false),
        owner(""),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        nativeValue(value),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject() :
        type(DynamicObjectType::NIL),
        isLocked(false),
        owner(""),
        functionValue(nullptr),
        arrayValue(nullptr),
        regexValue(nullptr),
        nativeValue(nullptr),
        stringValue(std::move("")),
        numberValue(0.0),
        boolValue(false) {}

    DynamicObject(const DynamicObject& other) :
        type(other.type),
        isLocked(false),
        owner(""),
        functionValue(other.functionValue),
        arrayValue(other.arrayValue),
        regexValue(other.regexValue),
        nativeValue(other.nativeValue),
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
    bool isNative() const;
    bool isString() const;
    bool isArray() const;
    bool isRegex() const;
    bool isBool() const;
    bool isNil() const;

    std::shared_ptr<FunctionDeclarationExpression> getCallable() const;
    std::shared_ptr<std::vector<DynamicObject>> getArray() const;
    std::shared_ptr<RegexWrapper> getRegex() const;
    NativeFunction getNativeFunction() const;
    const std::string& getString() const;
    double getNumber() const;
    bool getBool() const;

    void setArrayElement(
        std::shared_ptr<Token> reference,
        size_t index,
        std::shared_ptr<DynamicObject> object
    );

    DynamicObject callFromNative(
        std::shared_ptr<Token> address,
        SymbolTable& symtab,
        std::vector<DynamicObject> args
    );

    void lock();
    void unlock();

    bool hasLock();
    std::string ownerId() const;

    void own(std::string ownerUuid);

    std::string objectType();
    std::string toString();

    friend DynamicObject operator+(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator-(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator/(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator*(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator%(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator<(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator>(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator<=(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator>=(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator<<(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator>>(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator&(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator|(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator^(
        DynamicObject left,
        DynamicObject right
    );

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"

    friend DynamicObject operator&&(
        DynamicObject left,
        DynamicObject right
    );

    friend DynamicObject operator||(
        DynamicObject left,
        DynamicObject right
    );

    #pragma GCC diagnostic pop

    DynamicObject vectorAdd(DynamicObject arrayValue);
    DynamicObject vectorSub(DynamicObject arrayValue);
    DynamicObject vectorDiv(DynamicObject arrayValue);
    DynamicObject vectorMul(DynamicObject arrayValue);
    DynamicObject vectorRem(DynamicObject arrayValue);
    DynamicObject vectorBitwiseAnd(DynamicObject arrayValue);
    DynamicObject vectorBitwiseOr(DynamicObject arrayValue);
    DynamicObject vectorBitwiseXor(DynamicObject arrayValue);
    DynamicObject vectorShiftLeft(DynamicObject arrayValue);
    DynamicObject vectorShiftRight(DynamicObject arrayValue);
};

#endif
