/*
 * Copyright (c) 2025 - Nathanne Isip
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

#ifndef N8_INSTALLER_UTIL_HPP
#define N8_INSTALLER_UTIL_HPP

#include <cstdint>
#include <string>

bool IsElevated();

bool CreateDirectoryRecursive(const std::wstring& path);
bool ExtractFile(
    const std::wstring& path,
    const unsigned char* data,
    size_t size
);

bool SetEnvironmentVariablePersistent(
    const std::wstring& name,
    const std::wstring& value,
    bool systemWide
);
bool AddToSystemPath(const std::wstring& path);

bool CreateFileAssociation();
bool CreateUninstallEntry();

std::wstring GetInstallBase(bool isBin);
std::string WideStr2Utf8(const std::wstring& wstr);

#endif
