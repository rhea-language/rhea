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

#ifndef RHEA_STDLIB_CRYPT_CC
#define RHEA_STDLIB_CRYPT_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(crypt_md4);
RHEA_FUNC(crypt_validateMd4);

RHEA_FUNC(crypt_md5);
RHEA_FUNC(crypt_validateMd5);

RHEA_FUNC(crypt_sha224);
RHEA_FUNC(crypt_validateSha224);

RHEA_FUNC(crypt_sha256);
RHEA_FUNC(crypt_validateSha256);

RHEA_FUNC(crypt_sha384);
RHEA_FUNC(crypt_validateSha384);

RHEA_FUNC(crypt_sha512);
RHEA_FUNC(crypt_validateSha512);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
