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
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <conio.h>
#   include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#   include <termios.h>
#   include <unistd.h>
#endif

namespace N8Util {

#define TERMINAL_DEFAULT    std::string("\u001b[0m")
#define TERMINAL_STRING     std::string("\u001b[3;36m")
#define TERMINAL_KEYWORD    std::string("\u001b[0;32m")
#define TERMINAL_IDENTIFIER std::string("\u001b[1;37m")

class InputHighlighter final {
private:
    std::string prompt;
    std::unordered_set<std::string> keywords;
    std::vector<std::string> history;
    size_t history_index;

    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    HANDLE handle_console;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD original_mode;

    #elif defined(__linux__) || defined(__APPLE__)

    struct termios original_termios;

    #endif

    std::string colorizeInput(const std::string& input);
    bool isKeyword(const std::string& word);
    void clearLine();

public:
    InputHighlighter(std::string _prompt, const std::unordered_set<std::string>& _keywords) :
        prompt(_prompt),
        keywords(_keywords),
        history({}),
        history_index(0)
        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        , handle_console(GetStdHandle(STD_INPUT_HANDLE))
        , csbi({})
        , original_mode(0)
        #elif defined(__linux__) || defined(__APPLE__)
        , original_termios({})
        #endif
    { }

    std::string readInput();
};

}

#endif
