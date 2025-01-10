/*
 * Copyright (c) 2025 - Nathanne Isip
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

#include "n8std/Str.hpp"

#include <n8/ast/TerminativeSignal.hpp>

#include <cstdlib>

N8_FUNC(str_append) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject x = args.at(0),
        y = args.at(1);
    return DynamicObject(x.toString() + y.toString());
}

N8_FUNC(str_at) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0),
        index = args.at(1);
    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index parameter must be a number"
        );
    
    return DynamicObject(std::string(1, value.toString().at(
        static_cast<size_t>(index.getNumber())
    )));
}

N8_FUNC(str_contains) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0),
        substr = args.at(1);
    return DynamicObject(
        value.toString().find(substr.toString()) !=
            std::string::npos
    );
}

N8_FUNC(str_find) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0),
        str = args.at(1);
    return DynamicObject(static_cast<double>(
        source.toString().find(str.toString())
    ));
}

N8_FUNC(str_occurence) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0),
        str = args.at(1);
    std::string sourceStr = source.toString(),
        substr = str.toString();

    int count = 0;
    size_t pos = 0;

    while((pos = sourceStr.find(substr, pos)) != std::string::npos) {
        ++count;
        pos += substr.length();
    }

    return DynamicObject(static_cast<double>(count));
}

N8_FUNC(str_pop) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0);
    std::string str = source.toString();

    str.pop_back();
    return DynamicObject(str);
}

N8_FUNC(str_replace) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0),
        x = args.at(1),
        y = args.at(2);
    std::string sourceStr = source.toString(),
        oldStr = x.toString(),
        newStr = y.toString();

    size_t pos = sourceStr.find(oldStr);
    if(pos != std::string::npos)
        sourceStr.replace(pos, oldStr.length(), newStr);
    return DynamicObject(sourceStr);
}

N8_FUNC(str_replaceAll) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0),
        x = args.at(1),
        y = args.at(2);
    std::string sourceStr = source.toString(),
        oldStr = x.toString(),
        newStr = y.toString();

    size_t pos = 0;
    while((pos = sourceStr.find(oldStr, pos)) != std::string::npos) {
        sourceStr.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }

    return DynamicObject(sourceStr);
}

N8_FUNC(str_split) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0),
        delim = args.at(1);
    std::string sourceStr = source.toString(),
        delimStr = delim.toString();

    std::vector<DynamicObject> result;
    size_t start = 0, end = 0;

    while((end = sourceStr.find(delimStr, start)) != std::string::npos) {
        result.push_back(sourceStr.substr(start, end - start));
        start = end + delimStr.length();
    }

    result.push_back(DynamicObject(sourceStr.substr(start)));
    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        result
    ));
}

N8_FUNC(str_substring) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0),
        start = args.at(1),
        end = args.at(2);

    if(!start.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Start parameter must be of number type."
        );

    if(!end.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "End parameter must be of number type."
        );

    return DynamicObject(
        source.toString().substr(
            static_cast<size_t>(start.getNumber()),
            static_cast<size_t>(end.getNumber())
        )
    );
}

N8_FUNC(str_toArray) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0);
    std::string str = source.toString();
    std::vector<DynamicObject> contents;

    for(const char& character : str)
        contents.emplace_back(DynamicObject(
            std::string(1, character)
        ));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        contents
    ));
}

N8_FUNC(str_toBytes) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0);
    std::string str = source.toString();
    std::vector<DynamicObject> contents;

    for(const char& character : str)
        contents.emplace_back(DynamicObject(
            static_cast<double>(character)
        ));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        contents
    ));
}

N8_FUNC(str_trim) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0);
    std::string str = source.toString();

    const auto strBegin = str.find_first_not_of(" \t");
    if(strBegin == std::string::npos)
        return DynamicObject("");

    const auto strEnd = str.find_last_not_of(" \t"),
        strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}
