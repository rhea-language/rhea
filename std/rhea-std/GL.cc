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

#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/ast/expression/FunctionDeclarationExpression.hpp>
#include <rhea/util/RandomUtil.hpp>

#include <GLFW/glfw3.h>
#include <unordered_map>

RHEA_FUNC(gl_init) {
    return DynamicObject(glfwInit() == 1);
}

RHEA_FUNC(gl_initHint) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject hint = args.at(0),
        value = args.at(1);

    if(!hint.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for hint, got " +
                hint.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for value, got " +
                value.objectType()
        );

    glfwInitHint(
        static_cast<int>(hint.getNumber()),
        static_cast<int>(value.getNumber())
    );
    return {};
}

RHEA_FUNC(gl_versionString) {
    return DynamicObject(glfwGetVersionString());
}

RHEA_FUNC(gl_getError) {
    std::vector<DynamicObject> values;
    const char* errorMessage;

    values.emplace_back(DynamicObject(
        static_cast<double>(glfwGetError(&errorMessage))
    ));

    if(errorMessage == NULL)
        values.emplace_back(DynamicObject(""));
    else values.emplace_back(
        DynamicObject(std::string(errorMessage))
    );

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            values
        )
    );
}

RHEA_FUNC(gl_setErrorCallback) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject callback = args.at(0);
    if(!callback.isFunction())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting callback parameter to be of function type, "
                "got " + callback.objectType() + " instead"
        );

    static auto errorCallback = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetErrorCallback([](int error, const char* description) {
        std::vector<DynamicObject> params;
        params.emplace_back(DynamicObject(
            static_cast<double>(error)
        ));
        params.emplace_back(DynamicObject(
            std::string(description)
        ));

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
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject width = args.at(0),
        height = args.at(1),
        title = args.at(2);
    
    if(!width.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for width, got " +
                width.objectType()
        );
    if(!height.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for height, got " +
                height.objectType()
        );

    GLFWwindow* window = glfwCreateWindow(
        static_cast<int>(width.getNumber()),
        static_cast<int>(height.getNumber()),
        title.toString().c_str(),
        NULL, NULL
    );
    std::string key = RheaUtil::uniqueKey();

    if(!window)
        return {};

    windowMap[key] = window;
    return DynamicObject(key);
}

RHEA_FUNC(gl_makeContextCurrent) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();
    if(windowMap.find(keyStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window key does not exist"
        );

    glfwMakeContextCurrent(windowMap[keyStr]);
    return DynamicObject(keyStr);
}

RHEA_FUNC(gl_shouldCloseWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(windowMap.find(keyStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window key does not exist"
        );

    return DynamicObject(
        static_cast<bool>(
            glfwWindowShouldClose(windowMap[keyStr])
        )
    );
}

RHEA_FUNC(gl_clear) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject mask = args.at(0);
    if(!mask.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for mask, got " +
                mask.objectType()
        );

    glClear(static_cast<int>(mask.getNumber()));
    return {};
}

RHEA_FUNC(gl_clearColor) {
    if(args.size() != 4)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 4 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject red = args.at(0),
        green = args.at(1),
        blue = args.at(2),
        alpha = args.at(3);

    if(!red.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for red should be of number type, "
                "got " + red.objectType() + " instead"
        );

    if(!green.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for green should be of number type, "
                "got " + green.objectType() + " instead"
        );

    if(!blue.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for blue should be of number type, "
                "got " + blue.objectType() + " instead"
        );

    if(!alpha.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameter for alpha should be of number type, "
                "got " + alpha.objectType() + " instead"
        );

    glClearColor(
        red.getNumber(),
        green.getNumber(),
        blue.getNumber(),
        alpha.getNumber()
    );
    return {};
}

RHEA_FUNC(gl_swapBuffers) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();
    if(windowMap.find(keyStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window key does not exist"
        );

    glfwSwapBuffers(windowMap[keyStr]);
    return DynamicObject(keyStr);
}

RHEA_FUNC(gl_pollEvents) {
    if(args.size() != 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 0 argument, got " +
                std::to_string(args.size())
        );

    glfwPollEvents();
    return {};
}

RHEA_FUNC(gl_destroyWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();
    if(windowMap.find(keyStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window key does not exist"
        );

    glfwDestroyWindow(windowMap[keyStr]);
    return DynamicObject(keyStr);
}

RHEA_FUNC(gl_getMonitors) {
    int count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::vector<DynamicObject> results;
    for(size_t i = 0; i < (size_t) count; i++) {
        std::string key = RheaUtil::uniqueKey();

        monitorMap[key] = monitors[i];
        results.emplace_back(DynamicObject(key));
    }

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            results
        )
    );
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
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.find(keyStr) == monitorMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Monitor key does not exist"
        );

    GLFWmonitor* monitor = monitorMap[keyStr];
    int x = 0, y = 0;
    glfwGetMonitorPos(monitor, &x, &y);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(
        static_cast<double>(x)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(y)
    ));

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            results
        )
    );
}

RHEA_FUNC(gl_monitorWorkarea) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.find(keyStr) == monitorMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Monitor key does not exist"
        );

    GLFWmonitor* monitor = monitorMap[keyStr];
    int x = 0, y = 0, width = 0, height = 0;
    glfwGetMonitorWorkarea(monitor, &x, &y, &width, &height);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(
        static_cast<double>(x)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(y)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(width)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(height)
    ));

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            results
        )
    );
}

RHEA_FUNC(gl_monitorPhysicalSize) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.find(keyStr) == monitorMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Monitor key does not exist"
        );

    GLFWmonitor* monitor = monitorMap[keyStr];
    int width = 0, height = 0;
    glfwGetMonitorPhysicalSize(monitor, &width, &height);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(
        static_cast<double>(width)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(height)
    ));

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            results
        )
    );
}

RHEA_FUNC(gl_monitorContentScale) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.find(keyStr) == monitorMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Monitor key does not exist"
        );

    GLFWmonitor* monitor = monitorMap[keyStr];
    float width = 0, height = 0;
    glfwGetMonitorContentScale(monitor, &width, &height);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(width));
    results.emplace_back(DynamicObject(height));

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            results
        )
    );
}

RHEA_FUNC(gl_monitorName) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.find(keyStr) == monitorMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Monitor key does not exist"
        );

    GLFWmonitor* monitor = monitorMap[keyStr];
    return DynamicObject(std::string(
        glfwGetMonitorName(monitor)
    ));
}

RHEA_FUNC(gl_setMonitorCallback) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject callback = args.at(0);
    if(!callback.isFunction())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting callback parameter to be of function type, "
                "got " + callback.objectType() + " instead"
        );

    static auto cbFunction = callback.getCallable().get();
    static SymbolTable& symbols = symtab;

    glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) {
        std::string monitorKey = RheaUtil::uniqueKey();
        monitorMap[monitorKey] = monitor;

        std::vector<DynamicObject> params;
        params.emplace_back(DynamicObject(monitorKey));
        params.emplace_back(DynamicObject(
            static_cast<double>(event)
        ));

        cbFunction->call(symbols, params);
    });

    return callback;
}

RHEA_FUNC(gl_getVideoModes) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.find(keyStr) == monitorMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Monitor key does not exist"
        );

    int count = 0;
    GLFWmonitor* monitor = monitorMap[keyStr];
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);

    std::vector<DynamicObject> results;
    for(size_t i = 0; i < (size_t) count; i++) {
        std::vector<DynamicObject> item;
        item.emplace_back(DynamicObject(
            static_cast<double>(modes[i].width)
        ));
        item.emplace_back(DynamicObject(
            static_cast<double>(modes[i].height)
        ));
        item.emplace_back(DynamicObject(
            static_cast<double>(modes[i].redBits)
        ));
        item.emplace_back(DynamicObject(
            static_cast<double>(modes[i].greenBits)
        ));
        item.emplace_back(DynamicObject(
            static_cast<double>(modes[i].blueBits)
        ));
        item.emplace_back(DynamicObject(
            static_cast<double>(modes[i].refreshRate)
        ));

        results.emplace_back(
            std::make_shared<std::vector<DynamicObject>>(
                item
            )
        );
    }

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            results
        )
    );
}

RHEA_FUNC(gl_getVideoMode) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    if(monitorMap.find(keyStr) == monitorMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Monitor key does not exist"
        );

    GLFWmonitor* monitor = monitorMap[keyStr];
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    std::vector<DynamicObject> results;
    results.emplace_back(DynamicObject(
        static_cast<double>(mode->width)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(mode->height)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(mode->redBits)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(mode->greenBits)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(mode->blueBits)
    ));
    results.emplace_back(DynamicObject(
        static_cast<double>(mode->refreshRate)
    ));

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            results
        )
    );
}

#endif
