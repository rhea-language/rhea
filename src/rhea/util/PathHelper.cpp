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

#include <Rhea.hpp>
#include <rhea/util/PathHelper.hpp>

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

namespace RheaUtil {

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
std::string PathHelper::wstring2Utf8(const std::wstring& wstr) {
    if(wstr.empty())
        return "";

    int sizeNeeded = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(),
        (int) wstr.size(),
        nullptr, 0,
        nullptr,
        nullptr
    );

    if(sizeNeeded == 0)
        throw std::runtime_error(
            "Path helper failed to calculate size for UTF-8 conversion."
        );

    std::string utf8Str(
        static_cast<size_t>(sizeNeeded),
        0
    );
    int charsConverted = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(),
        (int) wstr.size(),
        &utf8Str[0],
        sizeNeeded,
        nullptr,
        nullptr
    );

    if(charsConverted == 0)
        throw std::runtime_error("Path helper failed to convert wstring to UTF-8.");

    utf8Str.resize(
        static_cast<size_t>(charsConverted)
    );

    return utf8Str;
}
#endif

std::string PathHelper::installationPath() {
    #if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)

    HKEY hKey;
    DWORD type;
    wchar_t buffer[32767];
    DWORD buffer_size_bytes = sizeof(buffer);

    if(RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Environment", 0,
        KEY_READ,
        &hKey
    ) == ERROR_SUCCESS) {
        if(RegQueryValueExW(
            hKey,
            L"RHEA_PATH",
            NULL,
            &type,
            reinterpret_cast<LPBYTE>(buffer),
            &buffer_size_bytes
        ) == ERROR_SUCCESS) {
            if(type == REG_SZ || type == REG_EXPAND_SZ) {
                RegCloseKey(hKey);

                size_t num_wchars = buffer_size_bytes / sizeof(wchar_t);
                if(num_wchars > 0 && buffer[num_wchars - 1] == L'\0');
                else if(num_wchars < (sizeof(buffer) / sizeof(wchar_t)))
                    buffer[num_wchars] = L'\0';
                else {
                    if(buffer[32767 - 1] != L'\0')
                        buffer[32767 - 1] = L'\0';
                }

                return PathHelper::wstring2Utf8(std::wstring(buffer));
            }
        }

        RegCloseKey(hKey);
    }

    const char* env_path = std::getenv(RHEA_ENV_PATH_NAME);
    if(env_path != nullptr)
        return std::string(env_path);

    return "";

    #else
    return std::getenv(RHEA_ENV_PATH_NAME);
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
    std::vector<std::string> rheaFiles;

    std::string directoryPath = PathHelper::installationPath() +
        FS_FILE_SEPARATOR + "modules" + FS_FILE_SEPARATOR +
        libraryName + "@" + libraryVersion +
        FS_FILE_SEPARATOR + "src";

    try {
        if(fs::exists(directoryPath) &&
            fs::is_directory(directoryPath))
            for(const auto& entry : fs::directory_iterator(directoryPath))
                if(entry.is_regular_file() && entry.path().extension() == ".rhea")
                    rheaFiles.push_back(entry.path().string());
    }
    catch(const std::exception& ex) {}

    return rheaFiles;
}

}