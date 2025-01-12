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

#include "n8std/GL.hpp"

#include <n8/ast/TerminativeSignal.hpp>
#include <n8/ast/expression/FunctionDeclarationExpression.hpp>
#include <n8/util/RandomUtil.hpp>

#include <GLFW/glfw3.h>
#include <unordered_map>

std::unordered_map<std::string, GLFWwindow*> windowMap;
std::unordered_map<std::string, GLFWmonitor*> monitorMap;
std::unordered_map<std::string, GLFWcursor*> cursorMap;

N8_FUNC(gl_init) {
    return DynamicObject(glfwInit() == 1);
}

N8_FUNC(gl_initHint) {
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
    return DynamicObject();
}

N8_FUNC(gl_versionString) {
    return DynamicObject(glfwGetVersionString());
}

N8_FUNC(gl_getError) {
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

N8_FUNC(gl_setErrorCallback) {
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

    return DynamicObject();
}

N8_FUNC(gl_terminate) {
    glfwTerminate();
    return DynamicObject();
}

N8_FUNC(gl_createWindow) {
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
    std::string uuid = N8Util::uniqueKey();

    if(!window)
        return DynamicObject();

    windowMap[uuid] = window;
    return DynamicObject(uuid);
}

N8_FUNC(gl_makeContextCurrent) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject uuid = args.at(0);
    std::string uuidStr = uuid.toString();
    if(windowMap.find(uuidStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window UUID does not exist"
        );

    glfwMakeContextCurrent(windowMap[uuidStr]);
    return DynamicObject(uuidStr);
}

N8_FUNC(gl_shouldCloseWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject uuid = args.at(0);
    std::string uuidStr = uuid.toString();

    if(windowMap.find(uuidStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window UUID does not exist"
        );

    return DynamicObject(
        static_cast<bool>(
            glfwWindowShouldClose(windowMap[uuidStr])
        )
    );
}

N8_FUNC(gl_clear) {
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
    return DynamicObject();
}

N8_FUNC(gl_swapBuffers) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject uuid = args.at(0);
    std::string uuidStr = uuid.toString();
    if(windowMap.find(uuidStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window UUID does not exist"
        );

    glfwSwapBuffers(windowMap[uuidStr]);
    return DynamicObject(uuidStr);
}

N8_FUNC(gl_pollEvents) {
    if(args.size() != 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 0 argument, got " +
                std::to_string(args.size())
        );

    glfwPollEvents();
    return DynamicObject();
}

N8_FUNC(gl_destroyWindow) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject uuid = args.at(0);
    std::string uuidStr = uuid.toString();
    if(windowMap.find(uuidStr) == windowMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Window UUID does not exist"
        );

    glfwDestroyWindow(windowMap[uuidStr]);
    return DynamicObject(uuidStr);
}
