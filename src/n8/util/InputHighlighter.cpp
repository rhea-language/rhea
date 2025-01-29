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

#include <n8/util/InputHighlighter.hpp>
#include <stack>

namespace N8Util {

static inline bool isBalanced(const std::string& input) {
    std::stack<char> stack;

    for(char ch : input) {
        if(ch == '(' || ch == '{' || ch == '[')
            stack.push(ch);
        else if(ch == ')' || ch == '}' || ch == ']') {
            if(stack.empty())
                return false;

            char top = stack.top();
            if((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '['))
                return false;

            stack.pop();
        }
    }

    return stack.empty();
}

InputHighlighter& InputHighlighter::operator=(const N8Util::InputHighlighter& other) {
    if(this != &other) {
        this->prompt = other.prompt;
        this->keywords = other.keywords;
        this->history = other.history;
        this->history_index = other.history_index;

        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

        this->handle_console = other.handle_console;
        this->csbi = other.csbi;
        this->original_mode = other.original_mode;

        #elif defined(__linux__) || defined(__APPLE__)

        this->original_termios = other.original_termios;

        #endif
    }

    return *this;
}

InputHighlighter::~InputHighlighter() {
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    SetConsoleMode(this->handle_console, this->original_mode);
    #elif defined(__linux__) || defined(__APPLE__)
    tcsetattr(STDIN_FILENO, TCSANOW, &this->original_termios);
    #endif
}

std::string InputHighlighter::colorizeInput(const std::string& input) {
    std::string currentWord, result;
    bool inQuote = false;
    char quoteChar = 0;

    for(size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if(c == '"' || c == '\'') {
            if(!inQuote) {
                inQuote = true;
                quoteChar = c;
                result += TERMINAL_STRING;
            }
            else if(c == quoteChar) {
                inQuote = false;
                result += TERMINAL_STRING;
            }
        }

        if(inQuote)
            result += std::string(1, c);
        else {
            if(std::isspace(c) || std::ispunct(c)) {
                if(this->isKeyword(currentWord))
                    result += TERMINAL_KEYWORD + currentWord + TERMINAL_DEFAULT;
                else if(c != '"' && c != '\'')
                    result += TERMINAL_IDENTIFIER + currentWord + TERMINAL_DEFAULT;

                currentWord.clear();
                result += c;
            }
            else currentWord += c;

            if(c == '"' || c == '\'')
                result += TERMINAL_DEFAULT;
        }
    }

    if(!currentWord.empty()) {
        if(this->isKeyword(currentWord))
            result += TERMINAL_KEYWORD + currentWord + TERMINAL_DEFAULT;
        else result += TERMINAL_IDENTIFIER + currentWord + TERMINAL_DEFAULT;
    }

    return result;
}

bool InputHighlighter::isKeyword(const std::string& word) {
    return std::find(
        keywords.begin(),
        keywords.end(),
        word
    ) != keywords.end();
}

void InputHighlighter::clearLine() {
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &this->csbi
    );

    DWORD written;
    COORD coordScreen = {0, this->csbi.dwCursorPosition.Y};

    FillConsoleOutputCharacter(
        GetStdHandle(STD_OUTPUT_HANDLE),
        ' ', 
        this->csbi.dwSize.X,
        coordScreen,
        &written
    );

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coordScreen
    );

    #else

    std::cout << "\u001b[K";

    #endif
}

std::string InputHighlighter::readInput() {
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    GetConsoleMode(this->handle_console, &this->original_mode);
    SetConsoleMode(
        this->handle_console,
        ENABLE_PROCESSED_INPUT  |
        ENABLE_MOUSE_INPUT      |
        ENABLE_EXTENDED_FLAGS
    );

    #elif defined(__linux__) || defined(__APPLE__)

    tcgetattr(STDIN_FILENO, &this->original_termios);

    struct termios raw = this->original_termios;
    raw.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    #endif

    std::string input;
    size_t cursor_pos = 0;

    std::cout << "\u001b[0m"
        << this->prompt
        << std::flush;

    this->history_index = this->history.size();
    while(true) {
        int c = getchar();
        switch(c) {
            case 127:
            case '\b':
                if(!input.empty() && cursor_pos > 0) {
                    input.erase(
                        input.begin() +
                            static_cast<long>(cursor_pos - 1)
                    );
                    cursor_pos--;
                    this->clearLine();

                    std::cout << "\r\u001b[0m"
                        << this->prompt
                        << this->colorizeInput(input);

                    for(size_t i = input.length(); i > cursor_pos; --i)
                        std::cout << std::string("\u001b[D");
                    std::flush(std::cout);
                }
                break;

            case '\n':
            case '\r':
                if(!input.empty())
                    this->history.push_back(input);

                #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
                SetConsoleMode(this->handle_console, this->original_mode);
                #elif defined(__linux__) || defined(__APPLE__)
                tcsetattr(STDIN_FILENO, TCSANOW, &this->original_termios);
                #endif

                std::cout << TERMINAL_DEFAULT << std::endl;
                return input;

            default:
                if(std::isprint(c)) {
                    input.insert(
                        input.begin() + static_cast<long>(cursor_pos),
                        static_cast<char>(c)
                    );

                    cursor_pos++;
                    this->clearLine();

                    std::cout << "\r\u001b[0m"
                        << this->prompt
                        << this->colorizeInput(input);

                    for(size_t i = input.length(); i > cursor_pos; --i)
                        std::cout << std::string("\u001b[D");
                    std::flush(std::cout);
                }
        }

        if(c == '\u001b') {
            getchar();

            switch(getchar()) {
                case 'A':
                    if(!this->history.empty() && this->history_index > 0) {
                        this->history_index--;

                        input = this->history[this->history_index];
                        cursor_pos = input.length();
                        this->clearLine();

                        std::cout << "\r\u001b[0m"
                            << this->prompt 
                            << this->colorizeInput(input)
                            << std::flush;
                    }
                    break;

                case 'B':
                    if(this->history_index < this->history.size() - 1) {
                        this->history_index++;

                        input = this->history[this->history_index];
                        cursor_pos = input.length();
                        this->clearLine();

                        std::cout << "\r\u001b[0m"
                            << this->prompt
                            << this->colorizeInput(input)
                            << std::flush;
                    }
                    else if(this->history_index == this->history.size() - 1) {
                        input.clear();
                        cursor_pos = 0;
                        this->clearLine();

                        std::cout << "\r\u001b[0m"
                            << this->prompt
                            << std::flush;
                    }
                    break;

                case 'C':
                    if(cursor_pos < input.length()) {
                        cursor_pos++;
                        this->clearLine();

                        std::cout << "\r\u001b[0m"
                            << this->prompt
                            << this->colorizeInput(input);

                        for(size_t i = input.length(); i > cursor_pos; --i)
                            std::cout << std::string("\u001b[D");
                        std::flush(std::cout);
                    }
                    break;

                case 'D':
                    if(cursor_pos > 0) {
                        cursor_pos--;
                        this->clearLine();

                        std::cout << "\r\u001b[0m"
                            << this->prompt
                            << this->colorizeInput(input);

                        for(size_t i = input.length(); i > cursor_pos; --i)
                            std::cout << std::string("\u001b[D");
                        std::flush(std::cout);
                    }
                    break;

                default:
                    break;
            }

            continue;
        }
    }
}

}
