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

#include "rhea-std/Chrono.hpp"

#include <rhea/ast/TerminativeSignal.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

RHEA_FUNC(chrono_now) {
    return DynamicObject(
        static_cast<double>(
            static_cast<long>(std::time(nullptr))
        )
    );
}

RHEA_FUNC(chrono_since) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    std::time_t current = std::time(nullptr);
    DynamicObject value = args.at(0);

    std::time_t since_tm = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    return DynamicObject(
        static_cast<double>(
            static_cast<long>(current - since_tm)
        )
    );
}

RHEA_FUNC(chrono_ms) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );

    auto point = std::chrono::system_clock::from_time_t(timestamp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        point.time_since_epoch()
    ) % 1000;

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(ms.count())
        )
    );
}

RHEA_FUNC(chrono_seconds) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_sec)
        )
    );
}

RHEA_FUNC(chrono_minutes) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_min)
        )
    );
}

RHEA_FUNC(chrono_hour) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_hour)
        )
    );
}

RHEA_FUNC(chrono_day) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_mday)
        )
    );
}

RHEA_FUNC(chrono_month) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_mon)
        )
    );
}

RHEA_FUNC(chrono_year) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_year)
        )
    );
}

RHEA_FUNC(chrono_dayNum) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_wday)
        )
    );
}

RHEA_FUNC(chrono_dayFromStart) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    return DynamicObject(
        static_cast<double>(
            static_cast<int>(tm->tm_yday)
        )
    );
}

RHEA_FUNC(chrono_clock) {
    return DynamicObject(
        static_cast<double>(clock())
    );
}

RHEA_FUNC(chrono_format) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0),
        format = args.at(1);
    std::time_t timestamp = static_cast<std::time_t>(
        static_cast<long>(value.getNumber())
    );
    std::tm *tm = std::gmtime(&timestamp);

    std::ostringstream oss;
    oss << std::put_time(tm, format.getString().c_str());

    return DynamicObject(oss.str());
}

RHEA_FUNC(chrono_toGmt) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0), gmt = args.at(1);
    return DynamicObject(
        value.getNumber() * gmt.getNumber() * 3600
    );
}
