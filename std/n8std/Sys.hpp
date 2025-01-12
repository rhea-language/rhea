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

#ifndef N8_STDLIB_SYS_CC
#define N8_STDLIB_SYS_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(sys_quickShell);

N8_FUNC(sys_shellConnect);
N8_FUNC(sys_shellWrite);
N8_FUNC(sys_shellReadOutput);
N8_FUNC(sys_shellReadError);
N8_FUNC(sys_shellForceExit);
N8_FUNC(sys_shellHasExited);
N8_FUNC(sys_shellExitCode);
N8_FUNC(sys_shellProcessId);
N8_FUNC(sys_shellClose);

N8_FUNC(sys_arch);
N8_FUNC(sys_platform);
N8_FUNC(sys_wordSize);
N8_FUNC(sys_endianess);
N8_FUNC(sys_cpuFeatures);
N8_FUNC(sys_sleep);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
