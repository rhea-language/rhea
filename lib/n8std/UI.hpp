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

#ifndef N8_STDLIB_IO_CC
#define N8_STDLIB_IO_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#if !(defined(__APPLE__) && (defined(__aarch64__) || defined(__arm64__)))

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(ui_init);
N8_FUNC(ui_quit);
N8_FUNC(ui_main);

N8_FUNC(ui_dialog_openFile);
N8_FUNC(ui_dialog_saveFile);
N8_FUNC(ui_dialog_messageBox);
N8_FUNC(ui_dialog_messageBoxError);

N8_FUNC(ui_window_create);
N8_FUNC(ui_window_onClosing);
N8_FUNC(ui_window_show);

N8_LIB_END

#else
#   pragma message("Libui is currently not supported for Apple M1 architecture.")
#endif

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
