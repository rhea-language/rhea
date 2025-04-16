/*
 * Copyright (c) 2025 - Nathanne Isip
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

#include "rhea-std/Archive.hpp"

#include <regex>
#include <Rhea.hpp>
 
RHEA_FUNC(regex_match) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    try {
        return DynamicObject(std::regex_match(
            text.toString(),
            pattern.getRegex()->getRegex()
        ));
    }
    catch(const std::regex_error& e) {
        throw TerminativeThrowSignal(
            std::move(address),
            "Matching error: " + std::string(e.what())
        );
    }

    return DynamicObject(false);
}

RHEA_FUNC(regex_isValidExpr) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0);
    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    try {
        std::regex $(pattern.getRegex()->getPattern());
        return DynamicObject(true);
    }
    catch(const std::regex_error& e) { }

    return DynamicObject(false);
}

RHEA_FUNC(regex_escapeRegex) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0);
    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::string origin = pattern.getRegex()
        ->getPattern();
    const std::string metaChars = R"(\.^$|()[]{}*+?)";
    std::string escaped;

    escaped.reserve(origin.size() * 2);
    for(char ch : origin) {
        if(metaChars.find(ch) != std::string::npos)
            escaped.push_back('\\');
        escaped.push_back(ch);
    }

    return DynamicObject(escaped);
}

RHEA_FUNC(regex_search) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    try {
        std::smatch match;
        std::string textStr = text.toString();

        std::regex_search(
            textStr,
            match,
            pattern.getRegex()->getRegex()
        );

        return DynamicObject(match.str());
    }
    catch(const std::regex_error& e) {
        throw TerminativeThrowSignal(
            std::move(address),
            "Matching error: " + std::string(e.what())
        );
    }

    return DynamicObject("");
}

RHEA_FUNC(regex_countMatches) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    int count = 0;
    std::string textStr = text.toString();

    try {
        std::regex expr = pattern.getRegex()
            ->getRegex();

        auto begin = std::sregex_iterator(
            textStr.begin(),
            textStr.end(),
            expr
        );
        auto end = std::sregex_iterator();

        count = std::distance(begin, end);
    }
    catch(const std::regex_error& e) {
        throw TerminativeThrowSignal(
            std::move(address),
            "Regex error: " + std::string(e.what())
        );
    }

    return DynamicObject(static_cast<float>(
        count
    ));
}

RHEA_FUNC(regex_findMatchPositions) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::shared_ptr<std::vector<DynamicObject>> positions =
        std::make_shared<std::vector<DynamicObject>>();

    try {
        std::string textStr = text.toString();
        std::regex expr = pattern.getRegex()->getRegex();
        size_t position = 0;

        auto start = std::sregex_iterator(
            textStr.begin(),
            textStr.end(),
            expr
        );
        auto end = std::sregex_iterator();

        for(std::sregex_iterator i = start; i != end; ++i) {
            std::smatch match = *i;
            std::shared_ptr<std::vector<DynamicObject>> pos =
                std::make_shared<std::vector<DynamicObject>>();

            pos->emplace_back(DynamicObject(
                static_cast<float>(match.position())
            ));
            pos->emplace_back(DynamicObject(
                static_cast<float>(match.length())
            ));

            positions->emplace_back(DynamicObject(pos));
        }
    }
    catch(const std::regex_error& err) {
        throw TerminativeThrowSignal(
            std::move(address),
            std::string(err.what())
        );
    }

    return DynamicObject(positions);
}

RHEA_FUNC(regex_findAllMatches) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::shared_ptr<std::vector<DynamicObject>> positions =
        std::make_shared<std::vector<DynamicObject>>();

    try {
        std::string textStr = text.toString();
        std::regex expr = pattern.getRegex()->getRegex();
        size_t position = 0;

        auto start = std::sregex_iterator(
            textStr.begin(),
            textStr.end(),
            expr
        );
        auto end = std::sregex_iterator();

        for(std::sregex_iterator i = start; i != end; ++i) {
            std::smatch match = *i;
            positions->emplace_back(DynamicObject(
                match.str()
            ));
        }
    }
    catch(const std::regex_error& err) {
        throw TerminativeThrowSignal(
            std::move(address),
            std::string(err.what())
        );
    }

    return DynamicObject(positions);
}

RHEA_FUNC(regex_replaceAll) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1),
        replacement = args.at(2);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::regex expr = pattern.getRegex()->getRegex();
    return DynamicObject(std::regex_replace(
        text.toString(),
        expr,
        replacement.toString()
    ));
}

RHEA_FUNC(regex_replaceFirst) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1),
        replacement = args.at(2);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::string textOutput = text.toString();
    std::regex expr = pattern.getRegex()->getRegex();

    std::smatch match;
    if(std::regex_search(textOutput, match, expr)) {
        size_t pos = match.position(),
            len = match.length();

        textOutput = textOutput.substr(0, pos) +
            replacement.toString() +
            textOutput.substr(pos + len);
    }

    return DynamicObject(textOutput);
}

RHEA_FUNC(regex_split) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::shared_ptr<std::vector<DynamicObject>> positions =
        std::make_shared<std::vector<DynamicObject>>();

    std::string textStr = text.toString();
    std::regex expr = pattern.getRegex()->getRegex();

    std::sregex_token_iterator iter(
        textStr.begin(),
        textStr.end(),
        expr
    );
    std::sregex_token_iterator end;

    for(; iter != end; ++iter)
        if(!iter->str().empty())
            positions->emplace_back(
                DynamicObject(iter->str())
            );

    return DynamicObject(positions);
}

RHEA_FUNC(regex_getCapturedGroups) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::shared_ptr<std::vector<DynamicObject>> groups =
        std::make_shared<std::vector<DynamicObject>>();

    std::string textStr = text.toString();
    std::regex expr = pattern.getRegex()->getRegex();
    std::smatch match;

    if(std::regex_search(textStr, match, expr))
        for(size_t i = 1; i < match.size(); ++i)
            groups->push_back(
                DynamicObject(match[i].str())
            );

    return DynamicObject(groups);
}

RHEA_FUNC(regex_getAllCapturedGroups) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject pattern = args.at(0),
        text = args.at(1);

    if(!pattern.isRegex())
        throw TerminativeThrowSignal(
            std::move(address),
            "Pattern argument should be of regex type, got " +
                pattern.objectType()
        );

    std::shared_ptr<std::vector<DynamicObject>> allGroups =
        std::make_shared<std::vector<DynamicObject>>();

    std::string textStr = text.toString();
    std::regex expr = pattern.getRegex()->getRegex();

    auto begin = std::sregex_iterator(
        textStr.begin(),
        textStr.end(),
        expr
    );
    std::sregex_iterator end;

    for(std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::shared_ptr<std::vector<DynamicObject>> groups =
            std::make_shared<std::vector<DynamicObject>>();

        for(size_t j = 1; j < match.size(); ++j)
            groups->push_back(DynamicObject(match[j].str()));
        allGroups->push_back(groups);
    }

    return allGroups;
}
