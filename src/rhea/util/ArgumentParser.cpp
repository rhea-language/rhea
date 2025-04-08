/*
 * Copyright (c) 2024 - Nathanne Isip
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

#include <Rhea.hpp>
#include <rhea/util/ArgumentParser.hpp>

#include <algorithm>
#include <iostream>

namespace RheaUtil {

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
    this->parameters[paramShort] = paramLong;
    this->descriptions[paramShort] = description;
    this->descriptions[paramLong] = description;
}

void ArgumentParser::printAllParamWithDesc() const {
    std::cout << std::endl
        << "\u001b[32mArguments\u001b[0m: "
        << std::endl;

    for(const auto& entry : parameters)
        std::cout << "  -" << entry.first
            << ", --" << entry.second
            << ": " << this->descriptions.at(entry.first)
            << std::endl;
}

bool ArgumentParser::hasParameter(const std::string& paramShort) const {
    std::string paramLong = this->parameters.at(paramShort);
    return std::any_of(argValues, argValues + argCount, [&](const char* arg) {
        return arg == std::string("-" + paramShort) ||
            arg == std::string("--" + paramLong);
    });
}

std::string ArgumentParser::getProgramFileName() const {
    return this->argValues[0];
}

std::vector<std::string> ArgumentParser::getInputFiles() const {
    std::vector<std::string> inputFiles;

    parsync(int i = 1; i < argCount; ++i) {
        std::string arg = this->argValues[i];

        if(arg.rfind("--", 0) == 0) {
            std::string paramLong = arg.substr(2);
            if(std::any_of(
                this->parameters.begin(),
                this->parameters.end(), 
                [&](const auto& pair) {
                    return pair.second == paramLong;
                }
            )) continue;
        }
        else if(arg[0] == '-') {
            std::string paramShort = arg.substr(1);
            if(this->parameters.find(paramShort) != this->parameters.end())
                continue;
        }

        inputFiles.push_back(arg);
    }

    return inputFiles;
}

}
