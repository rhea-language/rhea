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
#include <n8/util/VectorMath.hpp>

#include <cmath>
#include <string>

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

        for(size_t i = 0; i < array->size(); i++) {
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

DynamicObject operator+(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(left.getNumber() + right.getNumber());
    else if(left.isNumber() && right.isString())
        return DynamicObject(left.toString() + right.toString());
    else if((left.isNumber() && right.isArray()) ||
        (left.isString() && right.isArray()) ||
        (left.isRegex() && right.isArray()) ||
        (left.isBool() && right.isArray()) ||
        (left.isFunction() && right.isArray()) ||
        (left.isNil() && right.isArray())) {
        std::shared_ptr<std::vector<DynamicObject>> array = right.getArray();

        array->emplace_back(left);
        return DynamicObject(std::move(array));
    }
    else if(left.isNumber() && right.isRegex())
        return DynamicObject(std::make_shared<RegexWrapper>(
            std::string(std::to_string(left.getNumber())) +
                right.toString()
        ));
    else if((left.isString() && right.isNumber()) ||
        (left.isString() && right.isBool()) ||
        (left.isString() && right.isString()))
        return DynamicObject(
            left.toString() + right.toString()
        );
    else if(left.isString() && right.isRegex())
        return DynamicObject(std::make_shared<RegexWrapper>(
            left.toString() + right.getRegex()->getPattern()
        )
    );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::add(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Add operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }
    else if(left.isArray() && (
        right.isBool() ||
        right.isFunction() ||
        right.isNil() ||
        right.isNumber() ||
        right.isRegex() ||
        right.isString())
    ) {
        std::shared_ptr<std::vector<DynamicObject>> array = left.getArray();

        array->emplace_back(right);
        return DynamicObject(std::move(array));
    }
    else if((left.isRegex() && right.isNumber()) ||
        (left.isRegex() && right.isString()))
        return DynamicObject(std::make_shared<RegexWrapper>(
            left.getRegex()->getPattern() + right.toString()
        ));
    else if(left.isRegex() && right.isRegex())
        return DynamicObject(std::make_shared<RegexWrapper>(
            left.getRegex()->getPattern() +
            right.getRegex()->getPattern()
        ));
    else if(left.isBool() && right.isBool())
        return DynamicObject(static_cast<double>(
            static_cast<double>(left.getBool()) +
                static_cast<double>(right.getBool())
        ));
    else if(left.isBool() && right.isString())
        return DynamicObject(
            left.toString() + right.toString()
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(static_cast<double>(
            static_cast<double>(left.getBool()) +
                right.getNumber()
        ));
    else if(left.isBool() && right.isRegex())
        return DynamicObject(std::make_shared<RegexWrapper>(
            left.toString() +
                right.getRegex()->getPattern()
        ));
    else if((left.isRegex() && right.isNil()) ||
        (left.isBool() && right.isNil()) ||
        (left.isFunction() && right.isNil()) ||
        (left.isNative() && right.isNil()) ||
        (left.isNumber() && right.isNil()) ||
        (left.isString() && right.isNil()))
        return left;

    throw std::runtime_error(
        "Invalid '+' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator-(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNil())
        return right;
    else if(right.isNil())
        return left;
    else if(left.isNumber() && right.isNumber())
        return DynamicObject(
            left.getNumber() -
                right.getNumber()
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            left.getNumber() -
                static_cast<double>(right.getBool())
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(left.getBool())
                - right.getNumber()
        );
    else if(((left.isString() || left.isRegex()) && right.isNumber()) ||
        (left.isNumber() && (right.isString() || right.isRegex())) ||
        (left.isString() && right.isString())) {
        std::string leftVal = left.toString(),
            rightVal = right.toString();

        std::string subject, object;
        if(leftVal.size() >= rightVal.size()) {
            subject = leftVal;
            object = rightVal;
        }
        else {
            subject = rightVal;
            object = leftVal;
        }

        size_t index = 0;
        while((index = subject.find(subject, index)) != std::string::npos) {
            subject.replace(index, rightVal.size(), "");
            index++;
        }

        return DynamicObject(subject);
    }
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::sub(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Subtraction operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '-' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator/(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            left.getNumber() /
                right.getNumber()
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            left.getNumber() /
                static_cast<double>(right.getBool())
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(left.getBool()) /
                right.getNumber()
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(left.getBool()) /
                right.getBool()
        );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::div(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Division operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '/' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator*(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            left.getNumber() *
                right.getNumber()
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            left.getNumber() *
                static_cast<double>(right.getBool())
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(left.getBool()) *
                right.getNumber()
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(left.getBool()) *
                right.getNumber()
        );
    else if(left.isString() && right.isNumber()) {
        std::string str = left.getString();
        long count = static_cast<long>(right.getNumber());

        for(long i = 1; i < count; i++)
            str += str;

        return DynamicObject(str);
    }
    else if(left.isNumber() && right.isString()) {
        std::string str = right.getString();
        long count = static_cast<long>(left.getNumber());

        for(long i = 1; i < count; i++)
            str += str;

        return DynamicObject(str);
    }
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::mul(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Multiplication operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '*' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator%(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) %
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) %
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) %
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) %
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::rem(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Remainder operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '%' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator<(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            left.getNumber() <
                right.getNumber()
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            left.getNumber() <
                static_cast<double>(right.getBool())
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(left.getBool()) <
                right.getNumber()
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(left.getBool()) <
                right.getBool()
        );
    else if(left.isString() && right.isNumber())
        return DynamicObject(
            left.getString().size() <
                static_cast<std::size_t>(right.getNumber())
        );
    else if(left.isNumber() && right.isString())
        return DynamicObject(
            static_cast<std::size_t>(left.getNumber()) <
                right.getString().size()
        );

    throw std::runtime_error(
        "Invalid '<' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator>(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            left.getNumber() >
                right.getNumber()
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            left.getNumber() >
                static_cast<double>(right.getBool())
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(left.getBool()) >
                right.getNumber()
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(left.getBool()) >
                right.getBool()
        );
    else if(left.isString() && right.isNumber())
        return DynamicObject(
            left.getString().size() >
                static_cast<std::size_t>(right.getNumber())
        );
    else if(left.isNumber() && right.isString())
        return DynamicObject(
            static_cast<std::size_t>(left.getNumber()) >
                right.getString().size()
        );

    throw std::runtime_error(
        "Invalid '>' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator<=(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            left.getNumber() <=
                right.getNumber()
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            left.getNumber() <=
                static_cast<double>(right.getBool())
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(left.getBool()) <=
                right.getNumber()
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(left.getBool()) <=
                right.getBool()
        );
    else if(left.isString() && right.isNumber())
        return DynamicObject(
            left.getString().size() <=
                static_cast<std::size_t>(right.getNumber())
        );
    else if(left.isNumber() && right.isString())
        return DynamicObject(
            static_cast<std::size_t>(left.getNumber()) <=
                right.getString().size()
        );

    throw std::runtime_error(
        "Invalid '<=' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator>=(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            left.getNumber() >=
                right.getNumber()
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            left.getNumber() >=
                static_cast<double>(right.getBool())
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(left.getBool()) >=
                right.getNumber()
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(left.getBool()) >=
                right.getBool()
        );
    else if(left.isString() && right.isNumber())
        return DynamicObject(
            left.getString().size() >=
                static_cast<std::size_t>(right.getNumber())
        );
    else if(left.isNumber() && right.isString())
        return DynamicObject(
            static_cast<std::size_t>(left.getNumber()) >=
                right.getString().size()
        );

    throw std::runtime_error(
        "Invalid '>=' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator<<(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) <<
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) <<
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) <<
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) <<
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::shiftLeft(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Shift left operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '<<' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator>>(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) >>
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) >>
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) >>
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) >>
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::shiftRight(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Right shift operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '>>' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator&(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) &
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) &
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) &
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) &
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::bitwiseAnd(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Bitwise AND operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }
    throw std::runtime_error(
        "Invalid '&' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator|(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) |
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) |
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) |
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) |
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::bitwiseOr(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Bitwise OR operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '|' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

DynamicObject operator^(
    DynamicObject left,
    DynamicObject right
) {
    if(left.isNumber() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) ^
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isNumber() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getNumber()) ^
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isBool() && right.isNumber())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) ^
                    static_cast<long>(right.getNumber())
            )
        );
    else if(left.isBool() && right.isBool())
        return DynamicObject(
            static_cast<double>(
                static_cast<long>(left.getBool()) ^
                    static_cast<long>(right.getBool())
            )
        );
    else if(left.isArray() && right.isArray()) {
        if(N8Util::isNumberArray(*left.getArray()) &&
            N8Util::isNumberArray(*right.getArray())) {
            return DynamicObject(
                N8Util::vector2Object(N8Util::VectorMath::bitwiseXor(
                    N8Util::object2Vector(left),
                    N8Util::object2Vector(right)
                )
            ));
        }

        throw std::runtime_error(
            "Bitwise XOR operation for two (2) arrays cannot be done; "
            "not all elements are of number type."
        );
    }

    throw std::runtime_error(
        "Invalid '^' operator for object types; " +
            left.objectType() + " and " + right.objectType()
    );
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

DynamicObject operator&&(
    DynamicObject left,
    DynamicObject right
) {
    return DynamicObject(
        left.booleanEquivalent() &&
            right.booleanEquivalent()
    );
}

DynamicObject operator||(
    DynamicObject left,
    DynamicObject right
) {
    return DynamicObject(
        left.booleanEquivalent() ||
            right.booleanEquivalent()
    );
}

#pragma GCC diagnostic pop

DynamicObject DynamicObject::vectorAdd(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::addSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorSub(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::subSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorDiv(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::divSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorMul(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::mulSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorRem(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::remSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorBitwiseAnd(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::bitwiseAndSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorBitwiseOr(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::bitwiseOrSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorBitwiseXor(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::bitwiseXorSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorShiftLeft(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::shiftLeftSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}

DynamicObject DynamicObject::vectorShiftRight(DynamicObject arrayVal) {
    if(!arrayVal.isArray())
        throw std::runtime_error("Object value is not of array type.");
    else if(!this->isNumber())
        throw std::runtime_error("Single value is not of number type.");

    if(N8Util::isNumberArray(*arrayVal.getArray()))
        throw std::runtime_error("Object elements are not of number array type.");

    return DynamicObject(N8Util::vector2Object(
        N8Util::VectorMath::shiftRightSingle(
            this->getNumber(),
            N8Util::object2Vector(arrayVal)
        )
    ));
}
