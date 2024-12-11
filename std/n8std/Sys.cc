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
#include <map>
#include <myshell.hpp>

static std::map<std::string, std::shared_ptr<MyShell>> shellMap;

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
    std::string uuid = N8Util::generateUuid();

    shellMap[uuid] = std::make_shared<MyShell>(value.toString());
    return DynamicObject(uuid);
}
