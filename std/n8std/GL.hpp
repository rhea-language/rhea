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

#ifndef N8_STDLIB_GL_CC
#define N8_STDLIB_GL_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(gl_init);
N8_FUNC(gl_initHint);
N8_FUNC(gl_versionString);
N8_FUNC(gl_getError);
N8_FUNC(gl_setErrorCallback);
N8_FUNC(gl_terminate);
N8_FUNC(gl_createWindow);
N8_FUNC(gl_makeContextCurrent);
N8_FUNC(gl_shouldCloseWindow);
N8_FUNC(gl_clear);
N8_FUNC(gl_clearColor);
N8_FUNC(gl_swapBuffers);
N8_FUNC(gl_pollEvents);
N8_FUNC(gl_destroyWindow);
N8_FUNC(gl_getMonitors);
N8_FUNC(gl_primaryMonitor);
N8_FUNC(gl_monitorPosition);
N8_FUNC(gl_monitorWorkarea);
N8_FUNC(gl_monitorPhysicalSize);
N8_FUNC(gl_monitorContentScale);
N8_FUNC(gl_monitorName);
N8_FUNC(gl_setMonitorCallback);
N8_FUNC(gl_getVideoModes);
N8_FUNC(gl_getVideoMode);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
