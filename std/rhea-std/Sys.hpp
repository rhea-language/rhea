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

#ifndef RHEA_STDLIB_SYS_CC
#define RHEA_STDLIB_SYS_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(sys_quickShell);

RHEA_FUNC(sys_shellConnect);
RHEA_FUNC(sys_shellWrite);
RHEA_FUNC(sys_shellReadOutput);
RHEA_FUNC(sys_shellReadError);
RHEA_FUNC(sys_shellForceExit);
RHEA_FUNC(sys_shellHasExited);
RHEA_FUNC(sys_shellExitCode);
RHEA_FUNC(sys_shellProcessId);
RHEA_FUNC(sys_shellClose);

RHEA_FUNC(sys_arch);
RHEA_FUNC(sys_platform);
RHEA_FUNC(sys_wordSize);
RHEA_FUNC(sys_endianess);
RHEA_FUNC(sys_cpuFeatures);
RHEA_FUNC(sys_sleep);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
