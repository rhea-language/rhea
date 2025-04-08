/*
 * Copyright (c) 2025 - Nathanne Isip
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

#ifndef RHEA_STDLIB_UNSAFE_CC
#define RHEA_STDLIB_UNSAFE_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(unsafe_volatileRead8);
RHEA_FUNC(unsafe_volatileRead16);
RHEA_FUNC(unsafe_volatileRead32);

RHEA_FUNC(unsafe_volatileWrite8);
RHEA_FUNC(unsafe_volatileWrite16);
RHEA_FUNC(unsafe_volatileWrite32);

RHEA_FUNC(unsafe_registerSetBits);
RHEA_FUNC(unsafe_registerClearBits);
RHEA_FUNC(unsafe_registerToggleBits);
RHEA_FUNC(unsafe_registerTestBits);

RHEA_FUNC(unsafe_registerReadField);
RHEA_FUNC(unsafe_registerWriteField);

RHEA_FUNC(unsafe_memoryBarrier);
RHEA_FUNC(unsafe_readBarrier);
RHEA_FUNC(unsafe_writeBarrier);

RHEA_FUNC(unsafe_memoryFenceAcquire);
RHEA_FUNC(unsafe_memoryFenceRelease);
RHEA_FUNC(unsafe_memoryFenceSequential);

RHEA_FUNC(unsafe_enableInterrupts);
RHEA_FUNC(unsafe_disableInterrupts);
RHEA_FUNC(unsafe_setCpuAffinity);

RHEA_FUNC(unsafe_portRead8);
RHEA_FUNC(unsafe_portRead16);
RHEA_FUNC(unsafe_portRead32);

RHEA_FUNC(unsafe_portWrite8);
RHEA_FUNC(unsafe_portWrite16);
RHEA_FUNC(unsafe_portWrite32);

RHEA_FUNC(unsafe_inject);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
