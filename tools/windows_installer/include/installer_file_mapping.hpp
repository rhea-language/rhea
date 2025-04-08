/*
 * Copyright (c) 2025 - Nathanne Isip
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

#ifndef RHEA_INSTALLER_FILE_MAPPING_HPP
#define RHEA_INSTALLER_FILE_MAPPING_HPP

#include <cstdint>
#include <windows.h>

struct FileMapping {
    const char* arrayName;
    const unsigned char* data;
    size_t size;
    const wchar_t* originalPath;
};

#include <installer_embedded_files.hpp>
#include <installer_embedded_file_map.hpp>

#endif
