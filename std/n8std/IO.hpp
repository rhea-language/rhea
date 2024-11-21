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

#include "N8Library.hpp"

N8_LIB_START

N8_FUNC(io_print);
N8_FUNC(io_printLine);

N8_FUNC(io_readString);
N8_FUNC(io_readNumber);
N8_FUNC(io_readBoolean);

N8_FUNC(io_fileRead);
N8_FUNC(io_fileWrite);
N8_FUNC(io_fileSize);
N8_FUNC(io_filePerms);
N8_FUNC(io_fileCreationDate);
N8_FUNC(io_fileDelete);

N8_FUNC(io_folderCreate);
N8_FUNC(io_folderSize);
N8_FUNC(io_folderCreationDate);
N8_FUNC(io_folderDelete);

N8_FUNC(io_isFile);
N8_FUNC(io_isFolder);
N8_FUNC(io_listAllFiles);

N8_FUNC(io_exit);

N8_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
