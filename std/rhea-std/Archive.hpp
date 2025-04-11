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

#ifndef RHEA_STDLIB_ARCHIVE_CC
#define RHEA_STDLIB_ARCHIVE_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(archive_zip_open);
RHEA_FUNC(archive_zip_close);

RHEA_FUNC(archive_zip_compressionMethod);

RHEA_FUNC(archive_zip_setPassword);
RHEA_FUNC(archive_zip_setEncryption);

RHEA_FUNC(archive_zip_addFromFile);
RHEA_FUNC(archive_zip_addFromData);
RHEA_FUNC(archive_zip_addFromString);

RHEA_FUNC(archive_zip_deleteFile);
RHEA_FUNC(archive_zip_deleteDir);

RHEA_FUNC(archive_zip_getIndex);
RHEA_FUNC(archive_zip_hasFile);
RHEA_FUNC(archive_zip_renameFile);

RHEA_FUNC(archive_zip_entryCount);
RHEA_FUNC(archive_zip_listEntries);

RHEA_FUNC(archive_zip_getComment);
RHEA_FUNC(archive_zip_setComment);

RHEA_FUNC(archive_zip_getFileComment);
RHEA_FUNC(archive_zip_setFileComment);

RHEA_FUNC(archive_zip_fileDosTime);
RHEA_FUNC(archive_zip_discard);

RHEA_FUNC(archive_zip_registerProgressCallback);
RHEA_FUNC(archive_zip_registerCancelCallback);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
