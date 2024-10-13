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

#include <zhivo/util/ArgumentParser.hpp>

#include <algorithm>
#include <iostream>

ArgumentParser& ArgumentParser::operator=(const ArgumentParser& other) {
    if(this != &other) {
        this->argCount = other.argCount;
        this->argValues = other.argValues;
        this->parameters = other.parameters;
        this->descriptions = other.descriptions;
    }

    return *this;
}

void ArgumentParser::defineParameter(
    const std::string& paramShort,
    const std::string& paramLong,
    const std::string& description
) {
    parameters[paramShort] = paramLong;
    descriptions[paramShort] = description;
    descriptions[paramLong] = description;
}

void ArgumentParser::printAllParamWithDesc() const {
    std::cout << std::endl
        << "\u001b[32mArguments\u001b[0m: "
        << std::endl;

    for(const auto& entry : parameters)
        std::cout << "  -" << entry.first
            << ", --" << entry.second
            << ": " << descriptions.at(entry.first)
            << std::endl;
}

bool ArgumentParser::hasParameter(const std::string& paramShort) const {
    std::string paramLong = parameters.at(paramShort);
    return std::any_of(argValues, argValues + argCount, [&](const char* arg) {
        return arg == std::string("-" + paramShort) ||
            arg == std::string("--" + paramLong);
    });
}

std::vector<std::string> ArgumentParser::getInputFiles() const {
    std::vector<std::string> inputFiles;
    for(int i = 1; i < argCount; ++i) {
        std::string arg = argValues[i];
        if(arg[0] != '-' ||
            parameters.find(arg.substr(1)) == parameters.end())
            inputFiles.push_back(arg);
    }

    return inputFiles;
}
