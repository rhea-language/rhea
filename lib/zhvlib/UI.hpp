/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Zhivo.
 * 
 * Zhivo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Zhivo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Zhivo. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ZHIVO_STDLIB_IO_CC
#define ZHIVO_STDLIB_IO_CC

#include "ZhivoLibrary.hpp"

ZHIVO_LIB_START

ZHIVO_FUNC(ui_init);
ZHIVO_FUNC(ui_quit);
ZHIVO_FUNC(ui_main);

ZHIVO_FUNC(ui_dialog_openFile);
ZHIVO_FUNC(ui_dialog_saveFile);
ZHIVO_FUNC(ui_dialog_messageBox);
ZHIVO_FUNC(ui_dialog_messageBoxError);

ZHIVO_FUNC(ui_window_create);
ZHIVO_FUNC(ui_window_onClosing);
ZHIVO_FUNC(ui_window_show);

ZHIVO_LIB_END

#endif
