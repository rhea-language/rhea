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

#include <fstream>
#include <iostream>
#include <shlobj.h>
#include <string>
#include <vector>
#include <windows.h>

#include "embedded_files.h"

enum ConsoleColor {
    Blue = 1,
    Green = 2,
    Aqua = 3,
    Red = 4,
    Purple = 5,
    Yellow = 6,
    White = 7
};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(ConsoleColor color) {
    SetConsoleTextAttribute(hConsole, color);
}

void ResetColor() {
    SetConsoleTextAttribute(hConsole, White);
}

bool IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;

    if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);

        if(GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
            fRet = Elevation.TokenIsElevated;
    }

    if(hToken)
        CloseHandle(hToken);

    return fRet;
}

bool CreateDirectoryRecursive(const std::wstring& path) {
    size_t pos = 0;
    bool retVal = true;

    while(pos != std::wstring::npos) {
        pos = path.find_first_of(L"\\/", pos + 1);

        if(CreateDirectoryW(path.substr(0, pos).c_str(), NULL) == FALSE)
            if(GetLastError() != ERROR_ALREADY_EXISTS) {
                retVal = false;
                break;
            }
    }

    return retVal;
}

bool ExtractFile(const std::wstring& path, const unsigned char* data, size_t size) {
    size_t pos = path.find_last_of(L"\\/");
    if(pos != std::wstring::npos) {
        std::wstring dir = path.substr(0, pos);
        if(!CreateDirectoryRecursive(dir))
            return false;
    }

    HANDLE hFile = CreateFileW(
        path.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(hFile == INVALID_HANDLE_VALUE)
        return false;

    DWORD bytesWritten;
    BOOL result = WriteFile(
        hFile,
        data,
        static_cast<DWORD>(size),
        &bytesWritten,
        NULL
    );

    CloseHandle(hFile);
    return result && (bytesWritten == size);
}

bool SetEnvironmentVariablePersistent(const std::wstring& name, const std::wstring& value, bool systemWide) {
    HKEY hKey;
    HKEY root = systemWide ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    LPCWSTR keyPath = systemWide ? 
        L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment" :
        L"Environment";

    if(RegOpenKeyExW(root, keyPath, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
        return false;

    if(RegSetValueExW(
        hKey,
        name.c_str(),
        0, REG_SZ,
        reinterpret_cast<const BYTE*>(value.c_str()),
        (value.size() + 1) * sizeof(wchar_t)
    ) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }

    RegCloseKey(hKey);
    SendMessageTimeoutW(
        HWND_BROADCAST,
        WM_SETTINGCHANGE,
        0,
        reinterpret_cast<LPARAM>(L"Environment"),
        SMTO_ABORTIFHUNG,
        5000,
        NULL
    );

    return true;
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

std::wstring GetInstallBase(bool isBin) {
    wchar_t path[MAX_PATH];

    if(isBin) {
        if(SUCCEEDED(SHGetFolderPathW(
            NULL,
            CSIDL_PROGRAM_FILES,
            NULL,
            0, path
        ))) return std::wstring(path) + L"\\n8lang\\bin\\";
    }
    else if(SUCCEEDED(SHGetFolderPathW(
        NULL,
        CSIDL_MYDOCUMENTS,
        NULL,
        0, path
    ))) return std::wstring(path) + L"\\n8lang\\modules\\";

    return L"";
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

struct FileMapping {
    const char* arrayName;
    const unsigned char* data;
    size_t size;
    const wchar_t* originalPath;
};

#include "embedded_file_map.cpp"

int main() {
    if(!IsElevated()) {
        wchar_t path[MAX_PATH];

        GetModuleFileNameW(NULL, path, MAX_PATH);
        ShellExecuteW(
            NULL,
            L"runas",
            path,
            NULL,
            NULL,
            SW_SHOWNORMAL
        );

        return 0;
    }

    SetColor(Aqua);
    std::wcout << L"=== n8lang Installer ===" << std::endl;
    ResetColor();

    for(const auto& file : files) {
        std::wstring originalPath(file.originalPath);
        std::wstring targetPath;

        if(originalPath.find(L"dist\\n8lang\\bin") == 0)
            targetPath = GetInstallBase(true) + originalPath.substr(16);
        else if(originalPath.find(L"dist\\n8lang\\modules") == 0)
            targetPath = GetInstallBase(false) + originalPath.substr(20);
        else {
            SetColor(Red);
            std::wcerr << L"Invalid file path: " << originalPath << std::endl;
            ResetColor();

            continue;
        }

        if(!CreateDirectoryRecursive(targetPath.substr(0, targetPath.find_last_of(L'\\')))) {
            SetColor(Red);
            std::wcerr << L"Failed to create directory for: " << targetPath << std::endl;
            ResetColor();

            continue;
        }

        if(ExtractFile(targetPath, file.data, file.size)) {
            SetColor(Green);
            std::wcout << L"Installed: " << targetPath << std::endl;
            ResetColor();
        }
        else {
            SetColor(Red);
            std::wcerr << L"Failed to install: " << targetPath << std::endl;
            ResetColor();
        }
    }

    std::wstring n8Path = GetInstallBase(false).substr(0, GetInstallBase(false).find_last_of(L'\\'));
    n8Path = n8Path.substr(0, n8Path.find_last_of(L'\\'));

    if(SetEnvironmentVariablePersistent(L"N8_PATH", n8Path, false)) {
        SetColor(Green);
        std::wcout << L"Set N8_PATH to: " << n8Path << std::endl;
        ResetColor();
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to set N8_PATH" << std::endl;
        ResetColor();
    }

    std::wstring binPath = GetInstallBase(true).substr(0, GetInstallBase(true).find_last_of(L'\\'));
    if(AddToSystemPath(binPath)) {
        SetColor(Green);
        std::wcout << L"Added to PATH: " << binPath << std::endl;
        ResetColor();
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to add to PATH" << std::endl;
        ResetColor();
    }

    SetColor(Yellow);
    std::wcout << L"\nCreating file associations..." << std::endl;
    ResetColor();

    if(CreateFileAssociation()) {
        SetColor(Green);
        std::wcout << L"Associated .n8 files with n8.exe" << std::endl;
        ResetColor();
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to create file associations!" << std::endl;
        ResetColor();
    }

    SetColor(Yellow);
    std::wcout << L"Registering uninstaller..." << std::endl;
    ResetColor();

    if(CreateUninstallEntry()) {
        SetColor(Green);
        std::wcout << L"Created Add/Remove Programs entry" << std::endl;
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to create uninstall registry entry" << std::endl;
    }
    ResetColor();

    SetColor(Aqua);
    std::wcout << L"\nInstallation complete!" << std::endl;
    ResetColor();

    getchar();
    return 0;
}
