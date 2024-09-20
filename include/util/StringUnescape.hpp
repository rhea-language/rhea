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

#ifndef ZHIVO_UTIL_STRING_UNESCAPE_HPP
#define ZHIVO_UTIL_STRING_UNESCAPE_HPP

#include <regex>
#include <string>

namespace ZhivoUtil {

inline static std::string replaceEscapeSequences(const std::string& input) {
    std::string::const_iterator searchStart(input.cbegin());
    std::regex escapeRegex(R"(\\(n|r|t|a|b|v|f|e))");
    std::string result;
    std::smatch match;

    while(std::regex_search(searchStart, input.cend(), match, escapeRegex)) {
        result.append(searchStart, match[0].first);

        if(match[1] == "n")
            result.append("\n");
        else if(match[1] == "r")
            result.append("\r");
        else if(match[1] == "t")
            result.append("\t");
        else if(match[1] == "a")
            result.append("\a");
        else if(match[1] == "b")
            result.append("\b");
        else if(match[1] == "v")
            result.append("\v");
        else if(match[1] == "f")
            result.append("\f");
        else if(match[1] == "e")
            result.append("\u001b");

        searchStart = match[0].second;
    }

    result.append(searchStart, input.cend());
    return result;
}

};

#endif
