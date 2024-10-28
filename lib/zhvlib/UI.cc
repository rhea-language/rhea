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

#include "zhvlib/UI.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>

#include <stdlib.h>
#include <string.h>
#include <ui.h>

#include <zhivo/ast/expression/FunctionDeclarationExpression.hpp>

using EventCallback = std::function<int()>;
using WindowDictionary = std::unordered_map<std::string, uiWindow*>;

uiInitOptions initOptions;
WindowDictionary windowDictionary;

static std::unordered_map<std::string, EventCallback> windowCallbackMap;

static std::string generateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

    uint32_t data1 = dis(gen);
    uint16_t data2 = dis(gen) & 0xFFFF;
    uint16_t data3 = (dis(gen) & 0x0FFF) | 0x4000;
    uint16_t data4 = (dis(gen) & 0x3FFF) | 0x8000;
    uint16_t data5_1 = dis(gen) & 0xFFFF;
    uint16_t data5_2 = dis(gen) & 0xFFFF;

    std::stringstream ss;
    ss << std::hex << std::setfill('0')
        << std::setw(8) << data1 << "-"
        << std::setw(4) << data2 << "-"
        << std::setw(4) << data3 << "-"
        << std::setw(4) << data4 << "-"
        << std::setw(4) << data5_1
        << std::setw(4) << data5_2;

    return ss.str();
}

int windowCallbackWrapper(uiWindow* w, void* data) {
    std::string uuidStr(static_cast<char*>(data));
    auto it = windowCallbackMap.find(uuidStr);

    if(it != windowCallbackMap.end())
        return it->second();

    return 0;
}

ZHIVO_FUNC(ui_init) {
    memset(&initOptions, 0, sizeof (uiInitOptions));
    if(uiInit(&initOptions) != NULL) {
        abort();
        return DynamicObject(false);
    }

    return DynamicObject(true);
}

ZHIVO_FUNC(ui_quit) {
    uiQuit();
    return DynamicObject();
}

ZHIVO_FUNC(ui_main) {
    uiMain();
    return DynamicObject();
}

ZHIVO_FUNC(ui_dialog_openFile) {
    if(args.size() > 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject uuid = args.at(0);
    if(windowDictionary.find(uuid.toString()) == windowDictionary.end())
        throw std::runtime_error("Cannot find window with specified key");

    char* filePath = uiOpenFile(windowDictionary[uuid.toString()]);
    if(filePath == nullptr)
        return DynamicObject();

    return DynamicObject(std::string(filePath));
}

ZHIVO_FUNC(ui_dialog_saveFile) {
    if(args.size() > 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject uuid = args.at(0);
    if(windowDictionary.find(uuid.toString()) == windowDictionary.end())
        throw std::runtime_error("Cannot find window with specified key");

    char* filePath = uiSaveFile(windowDictionary[uuid.toString()]);
    if(filePath == nullptr)
        return DynamicObject();

    return DynamicObject(std::string(filePath));
}

ZHIVO_FUNC(ui_dialog_messageBox) {
    if(args.size() != 3)
        throw std::runtime_error(
            "Expecting 3 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject uuid = args.at(0),
        title = args.at(1),
        caption = args.at(2);

    if(windowDictionary.find(uuid.toString()) == windowDictionary.end())
        throw std::runtime_error("Cannot find window with specified key");

    uiMsgBox(
        windowDictionary[uuid.toString()],
        title.toString().c_str(),
        caption.toString().c_str()
    );
    return DynamicObject();
}

ZHIVO_FUNC(ui_dialog_messageBoxError) {
    if(args.size() != 3)
        throw std::runtime_error(
            "Expecting 3 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject uuid = args.at(0),
        title = args.at(1),
        caption = args.at(2);

    if(windowDictionary.find(uuid.toString()) == windowDictionary.end())
        throw std::runtime_error("Cannot find window with specified key");

    uiMsgBoxError(
        windowDictionary[uuid.toString()],
        title.toString().c_str(),
        caption.toString().c_str()
    );
    return DynamicObject();
}

ZHIVO_FUNC(ui_window_create) {
    if(args.size() != 4)
        throw std::runtime_error(
            "Expecting 4 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject title = args.at(0),
        width = args.at(1),
        height = args.at(2),
        hasMenubar = args.at(3);

    if(!width.isNumber())
        throw std::runtime_error("Parameter width must be a number");

    if(!height.isNumber())
        throw std::runtime_error("Parameter height must be a number");

    std::string key = generateUUID();
    if(windowDictionary.find(key) != windowDictionary.end())
        while(windowDictionary.find(key) != windowDictionary.end())
            key = generateUUID();

    windowDictionary[key] = uiNewWindow(
        title.toString().c_str(),
        width.getNumber(),
        height.getNumber(),
        hasMenubar.booleanEquivalent()
    );

    return DynamicObject(key);
}

ZHIVO_FUNC(ui_window_onClosing) {
    if(args.size() != 2)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject uuid = args.at(0),
        callback = args.at(1);
    uiWindow* window;

    if(!callback.isFunction())
        throw std::runtime_error("Parameter for callback is not a function");

    std::string uuidStr = uuid.toString();
    if(windowDictionary.find(uuidStr) != windowDictionary.end())
        window = windowDictionary[uuidStr];
    else throw std::runtime_error("Cannot find window with specified key");

    windowCallbackMap[uuidStr] = [callback, uuidStr, symtab]() -> int {
        std::shared_ptr<FunctionDeclarationExpression> func = callback.getCallable();
        DynamicObject value = func->call(
            symtab,
            {DynamicObject(uuidStr)}
        );

        return value.booleanEquivalent();
    };

    char* uuidData = new char[uuidStr.size() + 1];
    strcpy(uuidData, uuidStr.c_str());

    uiWindowOnClosing(
        window,
        windowCallbackWrapper,
        static_cast<void*>(uuidData)
    );

    return DynamicObject(uuidStr);
}

ZHIVO_FUNC(ui_window_show) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject uuid = args.at(0);
    std::string uuidStr = uuid.toString();

    if(windowDictionary.find(uuidStr) != windowDictionary.end())
        uiControlShow(uiControl(windowDictionary[uuidStr]));

    return DynamicObject(uuidStr);
}
