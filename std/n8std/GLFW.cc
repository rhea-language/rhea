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

#include "n8std/GLFW.hpp"

#include <n8/ast/TerminativeSignal.hpp>
#include <n8/util/RandomUtil.hpp>

#include <GLFW/glfw3.h>
#include <unordered_map>

#ifndef __TERMUX__

std::unordered_map<std::string, GLFWwindow*> windowMap;

N8_FUNC(glfw_init) {
    return DynamicObject(glfwInit() == 1);
}

N8_FUNC(glfw_terminate) {
    glfwTerminate();
    return DynamicObject();
}

N8_FUNC(glfw_createWindow) {
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
    std::string uuid = N8Util::generateUuid();

    if(!window)
        return DynamicObject();

    windowMap[uuid] = window;
    return DynamicObject(uuid);
}

N8_FUNC(glfw_makeContextCurrent) {
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

N8_FUNC(glfw_shouldCloseWindow) {
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

N8_FUNC(glfw_clear) {
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

N8_FUNC(glfw_swapBuffers) {
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

N8_FUNC(glfw_pollEvents) {
    if(args.size() != 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 0 argument, got " +
                std::to_string(args.size())
        );

    glfwPollEvents();
    return DynamicObject();
}

N8_FUNC(glfw_destroyWindow) {
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

#endif
