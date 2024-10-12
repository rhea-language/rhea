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

#ifndef ZHIVO_CORE_REGEX_WRAPPER_HPP
#define ZHIVO_CORE_REGEX_WRAPPER_HPP

#include <memory>
#include <regex>
#include <string>

class RegexWrapper {
private:
    std::string pattern;
    std::unique_ptr<std::regex> regex;

public:
    RegexWrapper(const std::string& _pattern) :
        pattern(std::move(_pattern)),
        regex(std::make_unique<std::regex>(this->pattern)) {}

    const std::string& getPattern() const;
    const std::regex getRegex() const;
};

#endif
