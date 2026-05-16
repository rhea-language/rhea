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

#ifndef RHEA_STDLIB_THREAD_CC
#define RHEA_STDLIB_THREAD_CC

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(thread_create);
RHEA_FUNC(thread_join);
RHEA_FUNC(thread_detach);
RHEA_FUNC(thread_sleep);
RHEA_FUNC(thread_yield);
RHEA_FUNC(thread_id);
RHEA_FUNC(thread_exit);

RHEA_FUNC(thread_mutex_create);
RHEA_FUNC(thread_mutex_lockMut);
RHEA_FUNC(thread_mutex_tryLock);
RHEA_FUNC(thread_mutex_unlock);
RHEA_FUNC(thread_mutex_destroy);

RHEA_FUNC(thread_condition_create);
RHEA_FUNC(thread_condition_hold);
RHEA_FUNC(thread_condition_signal);
RHEA_FUNC(thread_condition_broadcast);
RHEA_FUNC(thread_condition_destroy);

RHEA_LIB_END

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
