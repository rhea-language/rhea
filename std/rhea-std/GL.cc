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

#include "rhea-std/GL.hpp"

#ifndef __TERMUX__

#include <GLFW/glfw3.h>

#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/ast/expression/FunctionDeclarationExpression.hpp>
#include <rhea/util/RandomUtil.hpp>
#include <unordered_map>

RHEA_FUNC(gl_init) {
    return DynamicObject(glfwInit() == 1);
}

RHEA_FUNC(gl_initHint) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " + std::to_string(args.size()));

    DynamicObject hint = args.at(0), value = args.at(1);

    if(!hint.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for hint, got " + hint.objectType());

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for value, got " + value.objectType());

    glfwInitHint(static_cast<int>(hint.getNumber()),
                 static_cast<int>(value.getNumber()));
    return {};
}

RHEA_FUNC(gl_versionString) {
    return DynamicObject(glfwGetVersionString());
}

RHEA_FUNC(gl_getError) {
    std::vector<DynamicObject> values;
    const char* errorMessage;

    values.emplace_back(
        DynamicObject(static_cast<double>(glfwGetError(&errorMessage))));

    if(errorMessage == NULL)
        values.emplace_back(DynamicObject(""));
    else
        values.emplace_back(DynamicObject(std::string(errorMessage)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(values));
}

RHEA_FUNC(gl_setErrorCallback) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject callback = args.at(0);
    if(!callback.isFunction())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting callback parameter to be of function type, "
            "got " +
                callback.objectType() + " instead");

    static auto errorCallback = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetErrorCallback([](int error, const char* description) {
        std::vector<DynamicObject> params;
        params.emplace_back(DynamicObject(static_cast<double>(error)));
        params.emplace_back(DynamicObject(std::string(description)));

        errorCallback->call(symbols, params);
    });

    return {};
}

RHEA_FUNC(gl_terminate) {
    glfwTerminate();
    return {};
}

RHEA_FUNC(gl_createWindow) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " + std::to_string(args.size()));

    DynamicObject width = args.at(0), height = args.at(1), title = args.at(2);

    if(!width.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for width, got " + width.objectType());
    if(!height.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for height, got " + height.objectType());

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(width.getNumber()),
                                          static_cast<int>(height.getNumber()),
                                          title.toString().c_str(), NULL, NULL);
    std::string key = RheaUtil::uniqueKey();

    if(!window) return {};

    windowMap[key] = window;
    return DynamicObject(key);
}

RHEA_FUNC(gl_makeContextCurrent) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();
    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwMakeContextCurrent(windowMap[keyStr]);
    return DynamicObject(keyStr);
}

RHEA_FUNC(gl_shouldCloseWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    return DynamicObject(
        static_cast<bool>(glfwWindowShouldClose(windowMap[keyStr])));
}

RHEA_FUNC(gl_clear) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject mask = args.at(0);
    if(!mask.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for mask, got " + mask.objectType());

    glClear(static_cast<int>(mask.getNumber()));
    return {};
}

RHEA_FUNC(gl_clearColor) {
    if(args.size() != 4)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 4 argument, got " + std::to_string(args.size()));

    DynamicObject red = args.at(0), green = args.at(1), blue = args.at(2),
                  alpha = args.at(3);

    if(!red.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for red should be of number type, "
            "got " +
                red.objectType() + " instead");

    if(!green.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for green should be of number type, "
            "got " +
                green.objectType() + " instead");

    if(!blue.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for blue should be of number type, "
            "got " +
                blue.objectType() + " instead");

    if(!alpha.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for alpha should be of number type, "
            "got " +
                alpha.objectType() + " instead");

    glClearColor(red.getNumber(), green.getNumber(), blue.getNumber(),
                 alpha.getNumber());
    return {};
}

RHEA_FUNC(gl_swapBuffers) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();
    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwSwapBuffers(windowMap[keyStr]);
    return DynamicObject(keyStr);
}

RHEA_FUNC(gl_pollEvents) {
    if(args.size() != 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 0 argument, got " + std::to_string(args.size()));

    glfwPollEvents();
    return {};
}

RHEA_FUNC(gl_destroyWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();
    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwDestroyWindow(windowMap[keyStr]);
    return DynamicObject(keyStr);
}

RHEA_FUNC(gl_getMonitors) {
    int count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::vector<DynamicObject> results;
    for(size_t i = 0; i < (size_t)count; i++) {
        std::string key = RheaUtil::uniqueKey();

        monitorMap[key] = monitors[i];
        results.emplace_back(DynamicObject(key));
    }

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_primaryMonitor) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    std::string key = RheaUtil::uniqueKey();

    monitorMap[key] = monitor;
    return DynamicObject(key);
}

RHEA_FUNC(gl_monitorPosition) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Monitor key does not exist");

    GLFWmonitor* monitor = monitorMap[keyStr];
    int x = 0, y = 0;
    glfwGetMonitorPos(monitor, &x, &y);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(static_cast<double>(x)));
    results.emplace_back(DynamicObject(static_cast<double>(y)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_monitorWorkarea) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Monitor key does not exist");

    GLFWmonitor* monitor = monitorMap[keyStr];
    int x = 0, y = 0, width = 0, height = 0;
    glfwGetMonitorWorkarea(monitor, &x, &y, &width, &height);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(static_cast<double>(x)));
    results.emplace_back(DynamicObject(static_cast<double>(y)));
    results.emplace_back(DynamicObject(static_cast<double>(width)));
    results.emplace_back(DynamicObject(static_cast<double>(height)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_monitorPhysicalSize) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Monitor key does not exist");

    GLFWmonitor* monitor = monitorMap[keyStr];
    int width = 0, height = 0;
    glfwGetMonitorPhysicalSize(monitor, &width, &height);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(static_cast<double>(width)));
    results.emplace_back(DynamicObject(static_cast<double>(height)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_monitorContentScale) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Monitor key does not exist");

    GLFWmonitor* monitor = monitorMap[keyStr];
    float width = 0, height = 0;
    glfwGetMonitorContentScale(monitor, &width, &height);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(width));
    results.emplace_back(DynamicObject(height));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_monitorName) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Monitor key does not exist");

    GLFWmonitor* monitor = monitorMap[keyStr];
    return DynamicObject(std::string(glfwGetMonitorName(monitor)));
}

RHEA_FUNC(gl_setMonitorCallback) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject callback = args.at(0);
    if(!callback.isFunction())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting callback parameter to be of function type, "
            "got " +
                callback.objectType() + " instead");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) {
        std::string monitorKey = RheaUtil::uniqueKey();
        monitorMap[monitorKey] = monitor;

        std::vector<DynamicObject> params;
        params.emplace_back(DynamicObject(monitorKey));
        params.emplace_back(DynamicObject(static_cast<double>(event)));

        cbFunction->call(symbols, params);
    });

    return callback;
}

RHEA_FUNC(gl_getVideoModes) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Monitor key does not exist");

    int count = 0;
    GLFWmonitor* monitor = monitorMap[keyStr];
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);

    std::vector<DynamicObject> results;
    for(size_t i = 0; i < (size_t)count; i++) {
        std::vector<DynamicObject> item;
        item.emplace_back(DynamicObject(static_cast<double>(modes[i].width)));
        item.emplace_back(DynamicObject(static_cast<double>(modes[i].height)));
        item.emplace_back(DynamicObject(static_cast<double>(modes[i].redBits)));
        item.emplace_back(
            DynamicObject(static_cast<double>(modes[i].greenBits)));
        item.emplace_back(
            DynamicObject(static_cast<double>(modes[i].blueBits)));
        item.emplace_back(
            DynamicObject(static_cast<double>(modes[i].refreshRate)));

        results.emplace_back(
            std::make_shared<std::vector<DynamicObject>>(item));
    }

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_getVideoMode) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Monitor key does not exist");

    GLFWmonitor* monitor = monitorMap[keyStr];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(static_cast<double>(mode->width)));
    results.emplace_back(DynamicObject(static_cast<double>(mode->height)));
    results.emplace_back(DynamicObject(static_cast<double>(mode->redBits)));
    results.emplace_back(DynamicObject(static_cast<double>(mode->greenBits)));
    results.emplace_back(DynamicObject(static_cast<double>(mode->blueBits)));
    results.emplace_back(DynamicObject(static_cast<double>(mode->refreshRate)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_setWindowTitle) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), title = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwSetWindowTitle(windowMap[keyStr], title.toString().c_str());
    return {};
}

RHEA_FUNC(gl_getWindowPos) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    int x = 0, y = 0;
    glfwGetWindowPos(windowMap[keyStr], &x, &y);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(static_cast<double>(x)));
    results.emplace_back(DynamicObject(static_cast<double>(y)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_setWindowPos) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), x = args.at(1), y = args.at(2);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!x.isNumber() || !y.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Position coordinates must be numbers");

    glfwSetWindowPos(windowMap[keyStr], static_cast<int>(x.getNumber()),
                     static_cast<int>(y.getNumber()));
    return {};
}

RHEA_FUNC(gl_getWindowSize) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    int width = 0, height = 0;
    glfwGetWindowSize(windowMap[keyStr], &width, &height);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(static_cast<double>(width)));
    results.emplace_back(DynamicObject(static_cast<double>(height)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_setWindowSize) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), width = args.at(1), height = args.at(2);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!width.isNumber() || !height.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Window dimensions must be numbers");

    glfwSetWindowSize(windowMap[keyStr], static_cast<int>(width.getNumber()),
                      static_cast<int>(height.getNumber()));
    return {};
}

RHEA_FUNC(gl_setWindowAspectRatio) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), numer = args.at(1), denom = args.at(2);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!numer.isNumber() || !denom.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Aspect ratio values must be numbers");

    glfwSetWindowAspectRatio(windowMap[keyStr],
                             static_cast<int>(numer.getNumber()),
                             static_cast<int>(denom.getNumber()));
    return {};
}

RHEA_FUNC(gl_getWindowFrameSize) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    int left = 0, top = 0, right = 0, bottom = 0;
    glfwGetWindowFrameSize(windowMap[keyStr], &left, &top, &right, &bottom);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(static_cast<double>(left)));
    results.emplace_back(DynamicObject(static_cast<double>(top)));
    results.emplace_back(DynamicObject(static_cast<double>(right)));
    results.emplace_back(DynamicObject(static_cast<double>(bottom)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_iconifyWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwIconifyWindow(windowMap[keyStr]);
    return {};
}

RHEA_FUNC(gl_restoreWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwRestoreWindow(windowMap[keyStr]);
    return {};
}

RHEA_FUNC(gl_maximizeWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwMaximizeWindow(windowMap[keyStr]);
    return {};
}

RHEA_FUNC(gl_showWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwShowWindow(windowMap[keyStr]);
    return {};
}

RHEA_FUNC(gl_hideWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwHideWindow(windowMap[keyStr]);
    return {};
}

RHEA_FUNC(gl_focusWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    glfwFocusWindow(windowMap[keyStr]);
    return {};
}

RHEA_FUNC(gl_getWindowMonitor) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    GLFWmonitor* monitor = glfwGetWindowMonitor(windowMap[keyStr]);
    std::string monitorKey = RheaUtil::uniqueKey();
    monitorMap[monitorKey] = monitor;
    return DynamicObject(monitorKey);
}

RHEA_FUNC(gl_setWindowMonitor) {
    if(args.size() != 6)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 6 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), monitorKey = args.at(1), xpos = args.at(2),
                  ypos = args.at(3), width = args.at(4), height = args.at(5);
    std::string keyStr = key.toString(), monitorStr = monitorKey.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    GLFWmonitor* monitor = nullptr;
    if(monitorMap.count(monitorStr) == 1) monitor = monitorMap[monitorStr];

    if(!xpos.isNumber() || !ypos.isNumber() || !width.isNumber() ||
       !height.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Position and size must be numbers");

    glfwSetWindowMonitor(
        windowMap[keyStr], monitor, static_cast<int>(xpos.getNumber()),
        static_cast<int>(ypos.getNumber()), static_cast<int>(width.getNumber()),
        static_cast<int>(height.getNumber()), GLFW_DONT_CARE);
    return {};
}

RHEA_FUNC(gl_setWindowUserPointer) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), ptr = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    static std::unordered_map<std::string, DynamicObject> userPointerMap;
    userPointerMap[keyStr] = ptr;
    glfwSetWindowUserPointer(windowMap[keyStr], &userPointerMap[keyStr]);
    return {};
}

RHEA_FUNC(gl_getWindowUserPointer) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    static std::unordered_map<std::string, DynamicObject> userPointerMap;
    void* ptr = glfwGetWindowUserPointer(windowMap[keyStr]);
    if(ptr && userPointerMap.count(keyStr) == 1) return userPointerMap[keyStr];
    return {};
}

RHEA_FUNC(gl_setWindowPosCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowPosCallback(
        windowMap[keyStr], [](GLFWwindow* window, int xpos, int ypos) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<double>(xpos)));
            params.emplace_back(DynamicObject(static_cast<double>(ypos)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setWindowSizeCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowSizeCallback(
        windowMap[keyStr], [](GLFWwindow* window, int width, int height) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<double>(width)));
            params.emplace_back(DynamicObject(static_cast<double>(height)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setWindowCloseCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowCloseCallback(windowMap[keyStr], [](GLFWwindow* window) {
        std::vector<DynamicObject> params;
        cbFunction->call(symbols, params);
    });

    return {};
}

RHEA_FUNC(gl_setWindowRefreshCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowRefreshCallback(windowMap[keyStr], [](GLFWwindow* window) {
        std::vector<DynamicObject> params;
        cbFunction->call(symbols, params);
    });

    return {};
}

RHEA_FUNC(gl_setWindowFocusCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowFocusCallback(
        windowMap[keyStr], [](GLFWwindow* window, int focused) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<bool>(focused)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setWindowIconifyCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowIconifyCallback(
        windowMap[keyStr], [](GLFWwindow* window, int iconified) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<bool>(iconified)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setWindowMaximizeCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowMaximizeCallback(
        windowMap[keyStr], [](GLFWwindow* window, int maximized) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<bool>(maximized)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setWindowContentScaleCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetWindowContentScaleCallback(
        windowMap[keyStr], [](GLFWwindow* window, float xscale, float yscale) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(xscale));
            params.emplace_back(DynamicObject(yscale));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setWindowAspectRatioCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetFramebufferSizeCallback(
        windowMap[keyStr], [](GLFWwindow* window, int width, int height) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<double>(width)));
            params.emplace_back(DynamicObject(static_cast<double>(height)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setKeyCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetKeyCallback(
        windowMap[keyStr],
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<double>(key)));
            params.emplace_back(DynamicObject(static_cast<double>(scancode)));
            params.emplace_back(DynamicObject(static_cast<double>(action)));
            params.emplace_back(DynamicObject(static_cast<double>(mods)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setCharCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetCharCallback(
        windowMap[keyStr], [](GLFWwindow* window, unsigned int codepoint) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<double>(codepoint)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setCharModsCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetCharModsCallback(
        windowMap[keyStr],
        [](GLFWwindow* window, unsigned int codepoint, int mods) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<double>(codepoint)));
            params.emplace_back(DynamicObject(static_cast<double>(mods)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setMouseButtonCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetMouseButtonCallback(
        windowMap[keyStr],
        [](GLFWwindow* window, int button, int action, int mods) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<double>(button)));
            params.emplace_back(DynamicObject(static_cast<double>(action)));
            params.emplace_back(DynamicObject(static_cast<double>(mods)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setCursorPosCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetCursorPosCallback(windowMap[keyStr],
                             [](GLFWwindow* window, double xpos, double ypos) {
                                 std::vector<DynamicObject> params;
                                 params.emplace_back(DynamicObject(xpos));
                                 params.emplace_back(DynamicObject(ypos));
                                 cbFunction->call(symbols, params);
                             });

    return {};
}

RHEA_FUNC(gl_setCursorEnterCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetCursorEnterCallback(
        windowMap[keyStr], [](GLFWwindow* window, int entered) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(static_cast<bool>(entered)));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setScrollCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetScrollCallback(
        windowMap[keyStr],
        [](GLFWwindow* window, double xoffset, double yoffset) {
            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(xoffset));
            params.emplace_back(DynamicObject(yoffset));
            cbFunction->call(symbols, params);
        });

    return {};
}

RHEA_FUNC(gl_setDropCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), callback = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetDropCallback(windowMap[keyStr], [](GLFWwindow* window, int count,
                                              const char** paths) {
        std::vector<DynamicObject> pathList;
        for(int i = 0; i < count; i++)
            pathList.emplace_back(DynamicObject(std::string(paths[i])));
        cbFunction->call(
            symbols,
            {DynamicObject(
                std::make_shared<std::vector<DynamicObject>>(pathList))});
    });

    return {};
}

RHEA_FUNC(gl_getKey) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), keyVal = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!keyVal.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Key must be a number");

    return DynamicObject(static_cast<bool>(
        glfwGetKey(windowMap[keyStr], static_cast<int>(keyVal.getNumber()))));
}

RHEA_FUNC(gl_getKeyName) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), scancode = args.at(1);

    if(!key.isNumber() || !scancode.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Key and scancode must be numbers");

    const char* name = glfwGetKeyName(static_cast<int>(key.getNumber()),
                                      static_cast<int>(scancode.getNumber()));
    return DynamicObject(name ? std::string(name) : std::string(""));
}

RHEA_FUNC(gl_getKeyScancode) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);

    if(!key.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Key must be a number");

    return DynamicObject(static_cast<double>(
        glfwGetKeyScancode(static_cast<int>(key.getNumber()))));
}

RHEA_FUNC(gl_getMouseButton) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), button = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!button.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Button must be a number");

    return DynamicObject(static_cast<bool>(glfwGetMouseButton(
        windowMap[keyStr], static_cast<int>(button.getNumber()))));
}

RHEA_FUNC(gl_getCursorPos) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    double xpos = 0, ypos = 0;
    glfwGetCursorPos(windowMap[keyStr], &xpos, &ypos);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(xpos));
    results.emplace_back(DynamicObject(ypos));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_setCursorPos) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), xpos = args.at(1), ypos = args.at(2);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!xpos.isNumber() || !ypos.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Position must be numbers");

    glfwSetCursorPos(windowMap[keyStr], xpos.getNumber(), ypos.getNumber());
    return {};
}

RHEA_FUNC(gl_setCursorMode) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), mode = args.at(1);
    std::string keyStr = key.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    if(!mode.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Mode must be a number");

    glfwSetInputMode(windowMap[keyStr], GLFW_CURSOR,
                     static_cast<int>(mode.getNumber()));
    return {};
}

RHEA_FUNC(gl_createCursor) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 arguments, got " + std::to_string(args.size()));

    DynamicObject image = args.at(0), xhot = args.at(1), yhot = args.at(2);

    if(!xhot.isNumber() || !yhot.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Hotspot coordinates must be numbers");

    GLFWimage img;
    img.width = 16;
    img.height = 16;
    img.pixels = nullptr;

    GLFWcursor* cursor =
        glfwCreateCursor(&img, static_cast<int>(xhot.getNumber()),
                         static_cast<int>(yhot.getNumber()));
    std::string key = RheaUtil::uniqueKey();
    cursorMap[key] = cursor;
    return DynamicObject(key);
}

RHEA_FUNC(gl_createStandardCursor) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject shape = args.at(0);

    if(!shape.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Shape must be a number");

    GLFWcursor* cursor =
        glfwCreateStandardCursor(static_cast<int>(shape.getNumber()));
    std::string key = RheaUtil::uniqueKey();
    cursorMap[key] = cursor;
    return DynamicObject(key);
}

RHEA_FUNC(gl_destroyCursor) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(cursorMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Cursor key does not exist");

    glfwDestroyCursor(cursorMap[keyStr]);
    cursorMap.erase(keyStr);
    return {};
}

RHEA_FUNC(gl_setCursor) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0), cursorKey = args.at(1);
    std::string keyStr = key.toString(), cursorStr = cursorKey.toString();

    if(windowMap.count(keyStr) != 1)
        throw TerminativeThrowSignal(std::move(address),
                                     "Window key does not exist");

    GLFWcursor* cursor = nullptr;
    if(cursorMap.count(cursorStr) == 1) cursor = cursorMap[cursorStr];

    glfwSetCursor(windowMap[keyStr], cursor);
    return {};
}

RHEA_FUNC(gl_getJoystickPresent) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    return DynamicObject(static_cast<bool>(
        glfwJoystickPresent(static_cast<int>(jid.getNumber()))));
}

RHEA_FUNC(gl_getJoystickAxes) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    int count = 0;
    const float* axes =
        glfwGetJoystickAxes(static_cast<int>(jid.getNumber()), &count);

    std::vector<DynamicObject> results;
    for(int i = 0; i < count; i++) results.emplace_back(DynamicObject(axes[i]));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_getJoystickButtons) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    int count = 0;
    const unsigned char* buttons =
        glfwGetJoystickButtons(static_cast<int>(jid.getNumber()), &count);

    std::vector<DynamicObject> results;
    for(int i = 0; i < count; i++)
        results.emplace_back(DynamicObject(static_cast<bool>(buttons[i])));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_getJoystickHats) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    int count = 0;
    const unsigned char* hats =
        glfwGetJoystickHats(static_cast<int>(jid.getNumber()), &count);

    std::vector<DynamicObject> results;
    for(int i = 0; i < count; i++)
        results.emplace_back(DynamicObject(static_cast<double>(hats[i])));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_getJoystickName) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    const char* name = glfwGetJoystickName(static_cast<int>(jid.getNumber()));
    return DynamicObject(name ? std::string(name) : std::string(""));
}

RHEA_FUNC(gl_getJoystickGUID) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    const char* guid = glfwGetJoystickGUID(static_cast<int>(jid.getNumber()));
    return DynamicObject(guid ? std::string(guid) : std::string(""));
}

RHEA_FUNC(gl_setJoystickCallback) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject callback = args.at(0);

    if(!callback.isFunction())
        throw TerminativeThrowSignal(std::move(address),
                                     "Callback must be a function");

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetJoystickCallback([](int jid, int event) {
        std::vector<DynamicObject> params;
        params.emplace_back(DynamicObject(static_cast<double>(jid)));
        params.emplace_back(DynamicObject(static_cast<double>(event)));
        cbFunction->call(symbols, params);
    });

    return {};
}

RHEA_FUNC(gl_updateGamepadMappings) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject string = args.at(0);
    glfwUpdateGamepadMappings(string.toString().c_str());
    return {};
}

RHEA_FUNC(gl_getGamepadName) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    GLFWgamepadstate state;
    if(glfwGetGamepadState(static_cast<int>(jid.getNumber()), &state))
        return DynamicObject(
            std::string(glfwGetGamepadName(static_cast<int>(jid.getNumber()))));
    return DynamicObject("");
}

RHEA_FUNC(gl_getGamepadState) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject jid = args.at(0);

    if(!jid.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Joystick ID must be a number");

    GLFWgamepadstate state;
    if(!glfwGetGamepadState(static_cast<int>(jid.getNumber()), &state))
        return DynamicObject(false);

    std::vector<DynamicObject> buttons;
    for(int i = 0; i < 15; i++)
        buttons.emplace_back(
            DynamicObject(static_cast<bool>(state.buttons[i])));

    std::vector<DynamicObject> axes;
    for(int i = 0; i < 6; i++) axes.emplace_back(DynamicObject(state.axes[i]));

    std::vector<DynamicObject> results;
    results.emplace_back(
        DynamicObject(std::make_shared<std::vector<DynamicObject>>(buttons)));
    results.emplace_back(
        DynamicObject(std::make_shared<std::vector<DynamicObject>>(axes)));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(results));
}

RHEA_FUNC(gl_setClipboardString) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject string = args.at(0);
    glfwSetClipboardString(NULL, string.toString().c_str());
    return {};
}

RHEA_FUNC(gl_getClipboardString) {
    const char* string = glfwGetClipboardString(NULL);
    return DynamicObject(string ? std::string(string) : std::string(""));
}

RHEA_FUNC(gl_getTime) {
    return DynamicObject(glfwGetTime());
}

RHEA_FUNC(gl_setTime) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject time = args.at(0);

    if(!time.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Time must be a number");

    glfwSetTime(time.getNumber());
    return {};
}

RHEA_FUNC(gl_getTimerValue) {
    return DynamicObject(static_cast<double>(glfwGetTimerValue()));
}

RHEA_FUNC(gl_getTimerFrequency) {
    return DynamicObject(static_cast<double>(glfwGetTimerFrequency()));
}

RHEA_FUNC(gl_extensionSupported) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject extension = args.at(0);
    return DynamicObject(static_cast<bool>(
        glfwExtensionSupported(extension.toString().c_str())));
}

RHEA_FUNC(gl_getProcAddress) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject procname = args.at(0);
    GLFWglproc proc = glfwGetProcAddress(procname.toString().c_str());
    return DynamicObject(proc != nullptr);
}

#endif
