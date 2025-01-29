/*
 * Copyright (c) 2025 - Nathanne Isip
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

#ifndef N8_STDLIB_UNSAFE_CC
#define N8_STDLIB_UNSAFE_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(unsafe_volatileRead8);
N8_FUNC(unsafe_volatileRead16);
N8_FUNC(unsafe_volatileRead32);

N8_FUNC(unsafe_volatileWrite8);
N8_FUNC(unsafe_volatileWrite16);
N8_FUNC(unsafe_volatileWrite32);

N8_FUNC(unsafe_registerSetBits);
N8_FUNC(unsafe_registerClearBits);
N8_FUNC(unsafe_registerToggleBits);
N8_FUNC(unsafe_registerTestBits);

N8_FUNC(unsafe_registerReadField);
N8_FUNC(unsafe_registerWriteField);

N8_FUNC(unsafe_memoryBarrier);
N8_FUNC(unsafe_readBarrier);
N8_FUNC(unsafe_writeBarrier);

N8_FUNC(unsafe_memoryFenceAcquire);
N8_FUNC(unsafe_memoryFenceRelease);
N8_FUNC(unsafe_memoryFenceSequential);

N8_FUNC(unsafe_enableInterrupts);
N8_FUNC(unsafe_disableInterrupts);
N8_FUNC(unsafe_setCpuAffinity);

N8_FUNC(unsafe_portRead8);
N8_FUNC(unsafe_portRead16);
N8_FUNC(unsafe_portRead32);

N8_FUNC(unsafe_portWrite8);
N8_FUNC(unsafe_portWrite16);
N8_FUNC(unsafe_portWrite32);

N8_FUNC(unsafe_inject);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
