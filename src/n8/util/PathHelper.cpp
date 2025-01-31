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

#include <N8.hpp>
#include <n8/util/PathHelper.hpp>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <locale>
#include <string>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <shlobj.h>
#   include <windows.h>
#   include <winreg.h>
#endif

namespace N8Util {

std::string PathHelper::installationPath() {
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    HKEY hKey;
    DWORD type, size;
    wchar_t buffer[32767];

    if(RegOpenKeyExW(HKEY_CURRENT_USER, L"Environment", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        size = sizeof(buffer);
        if(RegQueryValueExW(hKey, L"N8_PATH", NULL, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS) {
            if(type == REG_SZ || type == REG_EXPAND_SZ) {
                RegCloseKey(hKey);

                std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv_x;
                return conv_x.to_bytes(std::wstring(buffer));
            }
        }

        RegCloseKey(hKey);
    }

    return "";

    #else
    return std::getenv(N8_ENV_PATH_NAME);
    #endif
}

bool PathHelper::isLibraryInstalled(
    std::string libraryName,
    std::string libraryVersion
) {
    namespace fs = std::filesystem;
    std::string path = PathHelper::installationPath() +
        FS_FILE_SEPARATOR + "modules" + FS_FILE_SEPARATOR +
        libraryName + "@" + libraryVersion +
        FS_FILE_SEPARATOR + "src";

    return fs::exists(path) &&
        fs::is_directory(path);
}

std::string PathHelper::findSharedLibrary(std::string name) {
    std::string
    #if defined(__APPLE__)
    extension = ".dylib";
    #elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    extension = ".so";
    #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    extension = ".dll";
    #endif

    std::string rawName = name + extension;
    if(std::filesystem::exists(rawName))
        return rawName;

    namespace fs = std::filesystem;
    std::string libPath = PathHelper::installationPath() +
        FS_FILE_SEPARATOR + "modules";

    for(const auto& modules : std::filesystem::directory_iterator(libPath)) {
        std::string modPath = modules.path().string() +
            FS_FILE_SEPARATOR + "lib";
        std::string folderName = fs::path(modules)
            .filename()
            .string();

        if(folderName.rfind(name, 0) == 0)
            for(const auto& entry : std::filesystem::directory_iterator(modPath))
                if(entry.is_regular_file()) {
                    std::string filename = entry
                        .path()
                        .filename()
                        .string();
                    std::string ext = fs::path(filename)
                        .extension()
                        .string();

                    if(filename.rfind(name, 0) == 0 && ext == extension)
                        return entry.path().string();
                }
    }

    throw std::runtime_error("Cannot find shared library: " + name);
}

std::vector<std::string> PathHelper::getLibraryFiles(
    std::string libraryName,
    std::string libraryVersion
) {
    namespace fs = std::filesystem;
    std::vector<std::string> n8Files;

    std::string directoryPath = PathHelper::installationPath() +
        FS_FILE_SEPARATOR + "modules" + FS_FILE_SEPARATOR +
        libraryName + "@" + libraryVersion +
        FS_FILE_SEPARATOR + "src";

    try {
        if(fs::exists(directoryPath) &&
            fs::is_directory(directoryPath))
            for(const auto& entry : fs::directory_iterator(directoryPath))
                if(entry.is_regular_file() && entry.path().extension() == ".n8")
                    n8Files.push_back(entry.path().string());
    }
    catch(const std::exception& ex) {}

    return n8Files;
}

}