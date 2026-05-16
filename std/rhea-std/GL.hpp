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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
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
RHEA_FUNC(gl_setWindowTitle);
RHEA_FUNC(gl_getWindowPos);
RHEA_FUNC(gl_setWindowPos);
RHEA_FUNC(gl_getWindowSize);
RHEA_FUNC(gl_setWindowSize);
RHEA_FUNC(gl_setWindowAspectRatio);
RHEA_FUNC(gl_getWindowFrameSize);
RHEA_FUNC(gl_iconifyWindow);
RHEA_FUNC(gl_restoreWindow);
RHEA_FUNC(gl_maximizeWindow);
RHEA_FUNC(gl_showWindow);
RHEA_FUNC(gl_hideWindow);
RHEA_FUNC(gl_focusWindow);
RHEA_FUNC(gl_getWindowMonitor);
RHEA_FUNC(gl_setWindowMonitor);
RHEA_FUNC(gl_setWindowUserPointer);
RHEA_FUNC(gl_getWindowUserPointer);
RHEA_FUNC(gl_setWindowPosCallback);
RHEA_FUNC(gl_setWindowSizeCallback);
RHEA_FUNC(gl_setWindowCloseCallback);
RHEA_FUNC(gl_setWindowRefreshCallback);
RHEA_FUNC(gl_setWindowFocusCallback);
RHEA_FUNC(gl_setWindowIconifyCallback);
RHEA_FUNC(gl_setWindowMaximizeCallback);
RHEA_FUNC(gl_setWindowContentScaleCallback);
RHEA_FUNC(gl_setWindowAspectRatioCallback);
RHEA_FUNC(gl_setKeyCallback);
RHEA_FUNC(gl_setCharCallback);
RHEA_FUNC(gl_setCharModsCallback);
RHEA_FUNC(gl_setMouseButtonCallback);
RHEA_FUNC(gl_setCursorPosCallback);
RHEA_FUNC(gl_setCursorEnterCallback);
RHEA_FUNC(gl_setScrollCallback);
RHEA_FUNC(gl_setDropCallback);
RHEA_FUNC(gl_getKey);
RHEA_FUNC(gl_getKeyName);
RHEA_FUNC(gl_getKeyScancode);
RHEA_FUNC(gl_getMouseButton);
RHEA_FUNC(gl_getCursorPos);
RHEA_FUNC(gl_setCursorPos);
RHEA_FUNC(gl_setCursorMode);
RHEA_FUNC(gl_createCursor);
RHEA_FUNC(gl_createStandardCursor);
RHEA_FUNC(gl_destroyCursor);
RHEA_FUNC(gl_setCursor);
RHEA_FUNC(gl_getJoystickPresent);
RHEA_FUNC(gl_getJoystickAxes);
RHEA_FUNC(gl_getJoystickButtons);
RHEA_FUNC(gl_getJoystickHats);
RHEA_FUNC(gl_getJoystickName);
RHEA_FUNC(gl_getJoystickGUID);
RHEA_FUNC(gl_setJoystickCallback);
RHEA_FUNC(gl_updateGamepadMappings);
RHEA_FUNC(gl_getGamepadName);
RHEA_FUNC(gl_getGamepadState);
RHEA_FUNC(gl_setClipboardString);
RHEA_FUNC(gl_getClipboardString);
RHEA_FUNC(gl_getTime);
RHEA_FUNC(gl_setTime);
RHEA_FUNC(gl_getTimerValue);
RHEA_FUNC(gl_getTimerFrequency);
RHEA_FUNC(gl_extensionSupported);
RHEA_FUNC(gl_getProcAddress);

RHEA_LIB_END

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif

#endif
