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

#include <n8/parser/OperatorsAndKeys.hpp>
#include <n8/util/InputHighlighter.hpp>

namespace N8Util {

InputHighlighter& InputHighlighter::operator=(const InputHighlighter& other) {
    if(this != &other) {
        this->inputBuffer = other.inputBuffer;

        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        this->hConsole = other.hConsole;
        #endif
    }

    return *this;
}

bool InputHighlighter::isKeyword(const std::string &word) {
    return std::find(
        OperatorsAndKeys::keywords.begin(),
        OperatorsAndKeys::keywords.end(),
        word
    ) != OperatorsAndKeys::keywords.end();
}

void InputHighlighter::highlightWord(const std::string &word) {
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD curPos = csbi.dwCursorPosition;

    curPos.X -= static_cast<SHORT>(word.length());
    SetConsoleCursorPosition(hConsole, curPos);

    if(this->isKeyword(word))
        SetConsoleTextAttribute(hConsole, RED_HIGHLIGHT);
    else SetConsoleTextAttribute(hConsole, CYAN_HIGHLIGHT);

    std::cout << word;
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR);

    #else

    std::cout << "\r\u001b[K";
    std::cout << inputBuffer;

    if(this->isKeyword(word))
        std::cout << RED_HIGHLIGHT << word << DEFAULT_COLOR;
    else std::cout << CYAN_HIGHLIGHT << word << DEFAULT_COLOR;

    std::cout.flush();

    #endif
}

char InputHighlighter::getCharacter() {
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    return static_cast<char>(_getch());

    #else

    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;

    #endif
}

std::string InputHighlighter::getInput() {
    bool inStringQuote = false;
    char quoteType = '\0', ch;

    inputBuffer.clear();
    while((ch = this->getCharacter()) != '\n') {
        #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
        if(ch == '\r')
            break;
        #endif

        if(ch == 127 || ch == '\b') {
            if(!inputBuffer.empty()) {
                if(inputBuffer.back() == quoteType) {
                    inStringQuote = false;
                    quoteType = '\0';
                }

                inputBuffer.pop_back();
                std::cout << "\b \b";
            }
        }
        else if(ch == '"' || ch == '\'') {
            if(!inStringQuote) {
                inStringQuote = true;
                quoteType = ch;
            }
            else if(quoteType == ch) {
                inStringQuote = false;
                quoteType = '\0';
            }

            inputBuffer += ch;
            std::cout << ch;

            this->highlightWord(std::string(1, ch));
        }
        else {
            inputBuffer += ch;
            std::cout << ch;

            if(inStringQuote)
                this->highlightWord(std::string(1, ch));
            else {
                size_t lastSpacePos = inputBuffer.find_last_of(' ');
                std::string currentWord = (lastSpacePos != std::string::npos)
                    ? inputBuffer.substr(lastSpacePos + 1)
                    : inputBuffer;

                if(this->isKeyword(currentWord))
                    this->highlightWord(currentWord);
            }
        }
    }

    std::cout << std::endl;
    return inputBuffer;
}

}
