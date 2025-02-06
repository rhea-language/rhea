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

#include "installer_util.hpp"

#include <shlobj.h>
#include <windows.h>
#include <winreg.h>

bool IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if(GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
            fRet = Elevation.TokenIsElevated;
    }
    if(hToken) CloseHandle(hToken);
    return fRet;
}

bool CreateDirectoryRecursive(const std::wstring& path) {
    size_t pos = 0;
    while(pos != std::wstring::npos) {
        pos = path.find_first_of(L"\\/", pos + 1);
        if(!CreateDirectoryW(path.substr(0, pos).c_str(), NULL) && 
            GetLastError() != ERROR_ALREADY_EXISTS)
            return false;
    }
    return true;
}

bool ExtractFile(const std::wstring& path, const unsigned char* data, size_t size) {
    size_t pos = path.find_last_of(L"\\/");
    if(pos != std::wstring::npos && 
        !CreateDirectoryRecursive(path.substr(0, pos)))
        return false;

    HANDLE hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE) return false;

    DWORD written;
    BOOL result = WriteFile(hFile, data, static_cast<DWORD>(size), &written, NULL);
    CloseHandle(hFile);
    return result && (written == size);
}

std::wstring GetInstallBase(bool isBin) {
    wchar_t path[MAX_PATH];
    if(isBin && SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILES, NULL, 0, path)))
        return std::wstring(path) + L"\\n8lang\\bin\\";
    if(!isBin && SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
        return std::wstring(path) + L"\\n8lang\\modules\\";
    return L"";
}

    bool SetEnvironmentVariablePersistent(const std::wstring& name, const std::wstring& value, bool systemWide) {
    HKEY root = systemWide ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    LPCWSTR keyPath = systemWide ? 
        L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment" :
        L"Environment";

    HKEY hKey;
    if(RegOpenKeyExW(root, keyPath, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
        return false;

    LSTATUS status = RegSetValueExW(
        hKey, name.c_str(), 0, REG_SZ,
        reinterpret_cast<const BYTE*>(value.c_str()),
        (value.size() + 1) * sizeof(wchar_t)
    );

    RegCloseKey(hKey);

    if(status == ERROR_SUCCESS) {
        SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                            reinterpret_cast<LPARAM>(L"Environment"),
                            SMTO_ABORTIFHUNG, 5000, NULL);
        return true;
    }
    return false;
}

bool AddToSystemPath(const std::wstring& path) {
    wchar_t currentPath[32767];
    DWORD length = GetEnvironmentVariableW(L"Path", currentPath, 32767);
    std::wstring newPath = currentPath;

    if(newPath.find(path) == std::wstring::npos) {
        if(!newPath.empty() && newPath.back() != L';')
            newPath += L';';
        newPath += path;
        return SetEnvironmentVariablePersistent(L"Path", newPath, true);
    }
    return true;
}

bool CreateFileAssociation() {
    HKEY hKey;
    std::wstring n8Path = GetInstallBase(true) + L"n8.exe";
    
    if(RegCreateKeyExW(
        HKEY_CLASSES_ROOT,
        L".n8", 0,
        NULL, 0,
        KEY_WRITE,
        NULL,
        &hKey,
        NULL
    ) == ERROR_SUCCESS) {
        std::wstring value = L"n8lang_script";

        RegSetValueExW(
            hKey,
            NULL, 0,
            REG_SZ,
            (const BYTE*) value.c_str(),
            (value.size() + 1) * sizeof(wchar_t)
        );
        RegCloseKey(hKey);
    }
    else return false;

    if(RegCreateKeyExW(HKEY_CLASSES_ROOT, L"n8lang_script", 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        std::wstring friendlyName = L"N8 Language Script";

        RegSetValueExW(
            hKey,
            NULL, 0,
            REG_SZ,
            (const BYTE*) friendlyName.c_str(),
            (friendlyName.size() + 1) * sizeof(wchar_t)
        );
        RegCloseKey(hKey);
    }
    else return false;

    if(RegCreateKeyExW(
        HKEY_CLASSES_ROOT,
        L"n8lang_script\\shell\\open\\command",
        0,
        NULL, 0,
        KEY_WRITE,
        NULL,
        &hKey,
        NULL
    ) == ERROR_SUCCESS) {
        std::wstring command = L"\"" + n8Path + L"\" \"%1\"";

        RegSetValueExW(
            hKey,
            NULL, 0,
            REG_SZ,
            (const BYTE*) command.c_str(),
            (command.size() + 1) * sizeof(wchar_t)
        );
        RegCloseKey(hKey);
    }
    else return false;

    return true;
}

bool CreateUninstallEntry() {
    HKEY hKey;

    std::wstring uninstallPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\N8Lang";
    std::wstring installPath = GetInstallBase(true).substr(0, GetInstallBase(true).find_last_of(L'\\'));
    std::wstring uninstallerPath = installPath + L"\\uninstaller.exe";
    std::wstring iconPath = installPath + L"\\bin\\n8.exe";

    if(RegCreateKeyExW(
        HKEY_LOCAL_MACHINE,
        uninstallPath.c_str(), 0,
        NULL, 0,
        KEY_WRITE,
        NULL,
        &hKey,
        NULL
    ) != ERROR_SUCCESS)
        return false;

    RegSetValueExW(
        hKey,
        L"DisplayName",
        0,
        REG_SZ,
        (const BYTE*) L"N8 Language",
        sizeof(L"N8 Language")
    );

    RegSetValueExW(
        hKey,
        L"UninstallString", 0,
        REG_SZ,
        (const BYTE*) uninstallerPath.c_str(),
        (uninstallerPath.size() + 1) * sizeof(wchar_t)
    );

    RegSetValueExW(
        hKey,
        L"InstallLocation", 0,
        REG_SZ,
        (const BYTE*) installPath.c_str(),
        (installPath.size() + 1) * sizeof(wchar_t)
    );

    RegSetValueExW(
        hKey,
        L"DisplayIcon", 0,
        REG_SZ,
        (const BYTE*) iconPath.c_str(),
        (iconPath.size() + 1) * sizeof(wchar_t)
    );

    RegSetValueExW(
        hKey,
        L"Publisher", 0,
        REG_SZ,
        (const BYTE*) L"Nathanne Isip",
        sizeof(L"Nathanne Isip")
    );

    RegSetValueExW(
        hKey,
        L"DisplayVersion", 0,
        REG_SZ,
        (const BYTE*) L"1.0.0",
        sizeof(L"1.0.0")
    );

    RegSetValueExW(
        hKey,
        L"URLInfoAbout", 0,
        REG_SZ,
        (const BYTE*) L"https://n8lang.netlify.app",
        sizeof(L"https://n8lang.netlify.app")
    );

    DWORD version = 100;
    RegSetValueExW(
        hKey,
        L"Version", 0,
        REG_DWORD,
        (const BYTE*) &version,
        sizeof(version)
    );

    RegCloseKey(hKey);
    return true;
}

std::string WideStr2Utf8(const std::wstring& wstr) {
    if(wstr.empty())
        return {};

    int size = WideCharToMultiByte(
        CP_UTF8, 0,
        &wstr[0],
        (int) wstr.size(), 
        nullptr, 0,
        nullptr,
        nullptr
    );
    std::string str(size, 0);

    WideCharToMultiByte(
        CP_UTF8, 0,
        &wstr[0],
        (int) wstr.size(),
        &str[0],
        size,
        nullptr,
        nullptr
    );
    return str;
}
