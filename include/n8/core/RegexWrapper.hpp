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

#ifndef N8_CORE_REGEX_WRAPPER_HPP
#define N8_CORE_REGEX_WRAPPER_HPP

#include <memory>
#include <regex>
#include <string>

class RegexWrapper final {
private:
    std::string pattern;
    std::shared_ptr<std::regex> regex;

public:
    RegexWrapper(const std::string& _pattern) :
        pattern(std::move(_pattern)),
        regex(std::make_shared<std::regex>(this->pattern)) {}

    const std::string& getPattern() const;
    const std::regex getRegex() const;
};

#endif
