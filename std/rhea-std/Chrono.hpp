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

#ifndef RHEA_STDLIB_CHRONO_CC
#define RHEA_STDLIB_CHRONO_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(chrono_now);
RHEA_FUNC(chrono_since);

RHEA_FUNC(chrono_ms);
RHEA_FUNC(chrono_seconds);
RHEA_FUNC(chrono_minutes);
RHEA_FUNC(chrono_hour);
RHEA_FUNC(chrono_day);
RHEA_FUNC(chrono_month);
RHEA_FUNC(chrono_year);

RHEA_FUNC(chrono_dayNum);
RHEA_FUNC(chrono_dayFromStart);

RHEA_FUNC(chrono_clock);
RHEA_FUNC(chrono_format);
RHEA_FUNC(chrono_toGmt);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
