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

#ifndef N8_UTIL_INPUT_HIGHLIGHTER_HPP
#define N8_UTIL_INPUT_HIGHLIGHTER_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <conio.h>
#   include <windows.h>
#else
#   include <termios.h>
#   include <unistd.h>
#endif

namespace N8Util {

class InputHighlighter final {
private:
    std::string inputBuffer;

    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    HANDLE hConsole;
    const WORD RED_HIGHLIGHT = FOREGROUND_RED | FOREGROUND_INTENSITY;
    const WORD CYAN_HIGHLIGHT = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    const WORD DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    #else

    const std::string RED_HIGHLIGHT = "\u001b[31m";
    const std::string CYAN_HIGHLIGHT = "\u001b[36m";
    const std::string DEFAULT_COLOR = "\u001b[0m";

    #endif

    bool isKeyword(const std::string &word);
    void highlightWord(const std::string &word);
    char getCharacter();

public:
    InputHighlighter() :
        inputBuffer("")
        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        , hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
        #endif
    { }

    InputHighlighter(const InputHighlighter& other) :
        inputBuffer(other.inputBuffer)
        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        , hConsole(other.hConsole)
        #endif
    { }

    InputHighlighter& operator=(const InputHighlighter& other);
    std::string getInput();
};

}

#endif
