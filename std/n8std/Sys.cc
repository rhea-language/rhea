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

#include "n8std/Sys.hpp"

#include <n8/ast/TerminativeSignal.hpp>
#include <n8/util/RandomUtil.hpp>

#include <cstdlib>
#include <myshell.hpp>
#include <unordered_map>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <windows.h>
#endif

std::unordered_map<std::string, std::shared_ptr<MyShell>> shellMap;

N8_FUNC(sys_quickShell) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    return DynamicObject(
        static_cast<double>(system(
            value.toString().c_str()
        ))
    );
}

N8_FUNC(sys_shellConnect) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = N8Util::uniqueKey();

    shellMap[uuid] = std::make_shared<MyShell>(
        value.toString()
    );
    return DynamicObject(uuid);
}

N8_FUNC(sys_shellWrite) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0),
        content = args.at(1);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    shellMap[uuid]->writeToShell(content.toString());
    return DynamicObject(uuid);
}

N8_FUNC(sys_shellReadOutput) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        shellMap[uuid]->readShellOutputStream()
    );
}

N8_FUNC(sys_shellReadError) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        shellMap[uuid]->readShellErrorStream()
    );
}

N8_FUNC(sys_shellForceExit) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    shellMap[uuid]->forceExit();
    return DynamicObject(uuid);
}

N8_FUNC(sys_shellHasExited) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(shellMap[uuid]->hasExited());
}

N8_FUNC(sys_shellExitCode) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        static_cast<double>(
            shellMap[uuid]->exitCode()
        )
    );
}

N8_FUNC(sys_shellProcessId) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        static_cast<double>(
            shellMap[uuid]->processId()
        )
    );
}

N8_FUNC(sys_shellClose) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.find(uuid) == shellMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    shellMap.erase(uuid);
    return DynamicObject(uuid);
}

N8_FUNC(sys_sleep) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    #if defined(_WIN32) || defined(_WIN64)
    Sleep(
        static_cast<DWORD>(value.getNumber())
    );
    #else
    std::this_thread::sleep_for(
        std::chrono::milliseconds(
            static_cast<int64_t>(value.getNumber())
        )
    );
    #endif

    return DynamicObject();
}
