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

#include "n8std/Array.hpp"

#include <n8/ast/TerminativeSignal.hpp>

#include <algorithm>
#include <vector>

N8_FUNC(array_create) {
    std::vector<DynamicObject> values;

    for(size_t i = 0; i < args.size(); i++) {
        DynamicObject item = args.at(i);
        values.emplace_back(item);
    }

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(values)
    );
}

N8_FUNC(array_clear) {
    if(args.empty())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting more than or equal 1 argument"
        );

    for(size_t i = 0; i < args.size(); i++) {
        DynamicObject item = args.at(i);

        if(item.isArray())
            item.getArray()->clear();
        else throw TerminativeThrowSignal(
            std::move(address),
            "Expecting an array argument, got " +
                item.objectType()
        );
    }

    return DynamicObject();
}

N8_FUNC(array_length) {
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
            "Expecting an array argument, got " +
                value.objectType()
        );

    return DynamicObject(static_cast<double>(
        value.getArray()->size()
    ));
}

N8_FUNC(array_reverse) {
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
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    std::reverse(array->begin(), array->end());

    return DynamicObject(array);
}

N8_FUNC(array_first) {
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
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    if(array->empty())
        return DynamicObject();
    return value.getArray()->at(0);
}

N8_FUNC(array_last) {
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
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    if(array->empty())
        return DynamicObject();
    return value.getArray()->back();
}

N8_FUNC(array_add) {
    if(args.size() > 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting greater than 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    for(size_t i = 1; i < args.size(); i++) {
        DynamicObject item = args.at(i);
        array->emplace_back(item);
    }

    return DynamicObject(array);
}

N8_FUNC(array_pushBack) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    DynamicObject item = args.at(1);

    array->push_back(item);
    return DynamicObject(array);
}

N8_FUNC(array_pushFront) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    DynamicObject item = args.at(1);

    array->push_back(item);
    std::rotate(
        array->rbegin(),
        array->rbegin() + 1,
        array->rend()
    );

    return DynamicObject(array);
}

N8_FUNC(array_assign) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 arguments, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    DynamicObject index = args.at(1),
        item = args.at(2);

    if(!index.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting a number as index argument, got " +
                index.objectType()
        );

    array->at(static_cast<size_t>(index.getNumber())) = item;
    return DynamicObject(array);
}

N8_FUNC(array_slice) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 arguments, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    DynamicObject from = args.at(1),
        to = args.at(2);

    if(!from.isNumber() || !to.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number type arguments for range parameters, got " +
                from.objectType() + " and " + to.objectType()
        );

    size_t fromNum = static_cast<size_t>(from.getNumber()),
        toNum = static_cast<size_t>(to.getNumber());

    if(fromNum > toNum)
        throw TerminativeThrowSignal(
            std::move(address),
            "Range end should be greater than range start."
        );

    std::vector<DynamicObject> retValue(toNum - fromNum + 1);
    std::copy(
        array->begin() + fromNum,
        array->begin() + toNum + 1,
        retValue.begin()
    );

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        retValue
    ));
}

N8_FUNC(array_remove);

N8_FUNC(array_removeAt);

N8_FUNC(array_removeAll);

N8_FUNC(array_removeSlice);

N8_FUNC(array_contains);

N8_FUNC(array_find);

N8_FUNC(array_at) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    if(!value.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting an array argument, got " +
                value.objectType()
        );

    auto array = value.getArray();
    DynamicObject index = args.at(1);
    if(!index.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting a number argument, got " +
                index.objectType()
        );

    DynamicObject item = array->at(
        static_cast<int>(index.getNumber())
    );
    return item;
}

N8_FUNC(array_join);

N8_FUNC(array_areAllString);

N8_FUNC(array_areAllNumber);

N8_FUNC(array_areAllFunction);
