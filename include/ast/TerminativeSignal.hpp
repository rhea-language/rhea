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

#ifndef ZHIVO_AST_TERMINATIVE_SIGNAL_HPP
#define ZHIVO_AST_TERMINATIVE_SIGNAL_HPP

#include <core/DynamicObject.hpp>
#include <parser/Token.hpp>

#include <stdexcept>

class TerminativeBreakSignal : public std::exception {
private:
    Token origin;

public:
    TerminativeBreakSignal(Token _origin) :
        origin(std::move(_origin)) {}

    Token getAddress() const;
};

class TerminativeContinueSignal : public std::exception {
private:
    Token origin;

public:
    TerminativeContinueSignal(Token _origin) :
        origin(std::move(_origin)) {}

    Token getAddress() const;
};

class TerminativeReturnSignal : public std::exception {
private:
    DynamicObject value;

public:
    TerminativeReturnSignal(DynamicObject _value) :
        value(std::move(_value)) {}

    DynamicObject getObject() const;
};

class TerminativeThrowSignal : public std::exception {
private:
    std::unique_ptr<Token> address;
    DynamicObject value;

public:
    TerminativeThrowSignal(std::unique_ptr<Token> _address, DynamicObject _value) :
        address(std::move(_address)),
        value(std::move(_value)) {}

    Token* getAddress() const;
    DynamicObject getObject() const;
};

#endif
