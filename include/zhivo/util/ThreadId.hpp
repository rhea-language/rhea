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

#ifndef ZHIVO_UTIL_THREAD_ID_HPP
#define ZHIVO_UTIL_THREAD_ID_HPP

#include <iomanip>
#include <sstream>
#include <thread>

namespace ZhivoUtil {

inline static std::string getThreadId(std::thread::id id) {
    std::ostringstream oss;
    oss << "0x" << std::hex << id;

    return oss.str();
}

};

#endif
