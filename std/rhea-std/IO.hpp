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

#ifndef RHEA_STDLIB_IO_CC
#define RHEA_STDLIB_IO_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(io_print);
RHEA_FUNC(io_printLine);

RHEA_FUNC(io_readString);
RHEA_FUNC(io_readNumber);
RHEA_FUNC(io_readBoolean);

RHEA_FUNC(io_fileRead);
RHEA_FUNC(io_fileWrite);
RHEA_FUNC(io_fileSize);
RHEA_FUNC(io_filePerms);
RHEA_FUNC(io_fileCreationDate);
RHEA_FUNC(io_fileDelete);

RHEA_FUNC(io_folderCreate);
RHEA_FUNC(io_folderSize);
RHEA_FUNC(io_folderCreationDate);
RHEA_FUNC(io_folderDelete);

RHEA_FUNC(io_isFile);
RHEA_FUNC(io_isFolder);
RHEA_FUNC(io_listAllFiles);

RHEA_FUNC(io_exit);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
