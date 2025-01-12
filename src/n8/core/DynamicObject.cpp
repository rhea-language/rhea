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

#include <N8.hpp>
#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/expression/FunctionDeclarationExpression.hpp>
#include <n8/core/DynamicObject.hpp>
#include <n8/core/RegexWrapper.hpp>
#include <n8/core/SymbolTable.hpp>

#include <cmath>

DynamicObject& DynamicObject::operator=(const DynamicObject& other) {
    if(this != &other) {
        this->type = other.type;
        this->isLocked = other.isLocked;
        this->owner = other.owner;

        this->numberValue = other.numberValue;
        this->boolValue = other.boolValue;
        this->stringValue = other.stringValue;

        this->arrayValue = other.arrayValue;
        this->functionValue = other.functionValue;
        this->regexValue = other.regexValue;
        this->nativeValue = other.nativeValue;
    }

    return *this;
}

DynamicObject& DynamicObject::operator=(DynamicObject&& other) {
    if(this != &other) {
        this->type = std::move(other.type);
        this->isLocked = std::move(other.isLocked);
        this->owner = std::move(other.owner);
        this->arrayValue = std::move(other.arrayValue);
        this->numberValue = std::move(other.numberValue);
        this->stringValue = std::move(other.stringValue);
        this->boolValue = std::move(other.boolValue);
        this->functionValue = std::move(other.functionValue);
        this->regexValue = std::move(other.regexValue);
        this->nativeValue = std::move(other.nativeValue);
    }

    return *this;
}

bool DynamicObject::operator==(const DynamicObject& other) {
    if(this->isNil() && other.isNil())
        return true;
    else if(this->isBool() && other.isBool())
        return this->getBool() == other.getBool();
    else if(this->isNumber() && other.isNumber())
        return std::fabs(this->getNumber() - other.getNumber()) <
            std::numeric_limits<double>::epsilon();
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

bool DynamicObject::operator!=(const DynamicObject& other) {
    return !(*this == other);
}

bool DynamicObject::isNumber() const {
    return this->type == DynamicObjectType::NUMBER;
}

bool DynamicObject::isNative() const {
    return this->type == DynamicObjectType::NATIVE;
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

NativeFunction DynamicObject::getNativeFunction() const {
    return this->nativeValue;
}

bool DynamicObject::booleanEquivalent() {
    return (this->isBool() && this->getBool()) ||
        (this->isNumber() && this->getNumber() < 0.0) ||
        (this->isString() && !this->getString().empty()) ||
        (this->isArray() && this->getArray()->size()) ||
        this->isFunction() || this->isRegex() ||
        this->isNative();
}

void DynamicObject::setArrayElement(
    std::shared_ptr<Token> reference,
    size_t index,
    std::shared_ptr<DynamicObject> object
) {
    if(!this->isArray())
        throw ASTNodeException(
            std::move(reference),
            "Subject value not an array."
        );

    if(index >= this->arrayValue->size())
        throw ASTNodeException(
            std::move(reference),
            "Index is out of bounds."
        );

    (*this->arrayValue)[index] = std::move(*object);
}

DynamicObject DynamicObject::callFromNative(
    std::shared_ptr<Token> address,
    SymbolTable& symtab,
    std::vector<DynamicObject> args
) {
    return !this->isNative() ?
        this->getCallable()->call(symtab, args) :
        this->getNativeFunction()(
            std::move(address),
            symtab,
            args
        );
}

void DynamicObject::lock() {
    this->isLocked = true;
}

void DynamicObject::unlock() {
    this->isLocked = false;
}

bool DynamicObject::hasLock() {
    return this->isLocked;
}

std::string DynamicObject::ownerId() const {
    return this->owner;
}

void DynamicObject::own(std::string ownerUuid) {
    this->owner = ownerUuid;
}

std::string DynamicObject::objectType() {
    if(this->isArray())
        return "array";
    else if(this->isBool())
        return "bool";
    else if(this->isString())
        return "string";
    else if(this->isNumber())
        return "number";
    else if(this->isNil())
        return "nil";
    else if(this->isFunction())
        return "function";
    else if(this->isRegex())
        return "regex";
    else if(this->isNative())
        return "native";

    return "unknown";
}

std::string DynamicObject::toString() {
    if(this->isNil())
        return "nil";
    else if(this->isNumber()) {
        std::string numstr = std::to_string(this->getNumber());
        size_t dotPos = numstr.find('.');

        if(dotPos != std::string::npos) {
            numstr.erase(numstr.find_last_not_of('0') + 1);

            if(numstr.back() == '.')
                numstr.pop_back();
        }

        return numstr;
    }
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

        parsync(size_t i = 0; i < array->size(); i++) {
            result += array->at(i).toString();

            if(i < array->size() - 1)
                result += ", ";
        }

        result += "]";
        return result;
    }
    else if(this->isNative())
        return "{{native_func}}";

    return "{untyped}";
}
