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

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "ZhivoLibrary.hpp"

ZHIVO_LIB_START

ZHIVO_FUNC(io_print);
ZHIVO_FUNC(io_printLine);

ZHIVO_FUNC(io_readString);
ZHIVO_FUNC(io_readNumber);
ZHIVO_FUNC(io_readBoolean);

ZHIVO_FUNC(io_fileRead);
ZHIVO_FUNC(io_fileWrite);
ZHIVO_FUNC(io_fileSize);
ZHIVO_FUNC(io_filePerms);
ZHIVO_FUNC(io_fileCreationDate);
ZHIVO_FUNC(io_fileDelete);

ZHIVO_FUNC(io_folderCreate);
ZHIVO_FUNC(io_folderSize);
ZHIVO_FUNC(io_folderCreationDate);
ZHIVO_FUNC(io_folderDelete);

ZHIVO_FUNC(io_isFile);
ZHIVO_FUNC(io_isFolder);
ZHIVO_FUNC(io_listAllFiles);

ZHIVO_FUNC(io_exit);

ZHIVO_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
