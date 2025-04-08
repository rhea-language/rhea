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

#ifndef RHEA_STDLIB_GL_CC
#define RHEA_STDLIB_GL_CC

#ifndef __TERMUX__

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(gl_init);
RHEA_FUNC(gl_initHint);
RHEA_FUNC(gl_versionString);
RHEA_FUNC(gl_getError);
RHEA_FUNC(gl_setErrorCallback);
RHEA_FUNC(gl_terminate);
RHEA_FUNC(gl_createWindow);
RHEA_FUNC(gl_makeContextCurrent);
RHEA_FUNC(gl_shouldCloseWindow);
RHEA_FUNC(gl_clear);
RHEA_FUNC(gl_clearColor);
RHEA_FUNC(gl_swapBuffers);
RHEA_FUNC(gl_pollEvents);
RHEA_FUNC(gl_destroyWindow);
RHEA_FUNC(gl_getMonitors);
RHEA_FUNC(gl_primaryMonitor);
RHEA_FUNC(gl_monitorPosition);
RHEA_FUNC(gl_monitorWorkarea);
RHEA_FUNC(gl_monitorPhysicalSize);
RHEA_FUNC(gl_monitorContentScale);
RHEA_FUNC(gl_monitorName);
RHEA_FUNC(gl_setMonitorCallback);
RHEA_FUNC(gl_getVideoModes);
RHEA_FUNC(gl_getVideoMode);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif

#endif
