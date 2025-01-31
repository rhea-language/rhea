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
#include <string>
#include <vector>

#include <shlwapi.h>
#include <shlobj.h>
#include <windows.h>

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

bool DeleteDirectory(const std::wstring& path) {
    WIN32_FIND_DATAW findData;
    HANDLE hFind;

    std::wstring searchPattern = path + L"\\*.*";
    hFind = FindFirstFileW(searchPattern.c_str(), &findData);

    if(hFind == INVALID_HANDLE_VALUE)
        return false;

    do {
        if(wcscmp(findData.cFileName, L".") == 0 || 
            wcscmp(findData.cFileName, L"..") == 0
        ) continue;

        std::wstring filePath = path + L"\\" + findData.cFileName;
        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if(!DeleteDirectory(filePath)) {
                FindClose(hFind);
                return false;
            }
        }
        else {
            if(!DeleteFileW(filePath.c_str())) {
                FindClose(hFind);
                return false;
            }
        }
    }
    while(FindNextFileW(hFind, &findData));

    FindClose(hFind);
    return RemoveDirectoryW(path.c_str());
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

bool RemoveEnvironmentVariablePersistent(const std::wstring& name, bool systemWide) {
    HKEY hKey;
    HKEY root = systemWide ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    LPCWSTR keyPath = systemWide ? 
        L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment" :
        L"Environment";

    if(RegOpenKeyExW(root, keyPath, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
        return false;

    LSTATUS status = RegDeleteValueW(hKey, name.c_str());
    RegCloseKey(hKey);

    if(status != ERROR_SUCCESS && status != ERROR_FILE_NOT_FOUND)
        return false;

    SendMessageTimeoutW(
        HWND_BROADCAST,
        WM_SETTINGCHANGE, 0,
        reinterpret_cast<LPARAM>(L"Environment"),
        SMTO_ABORTIFHUNG,
        5000,
        NULL
    );

    return true;
}

bool RemoveFromSystemPath(const std::wstring& pathToRemove) {
    wchar_t currentPath[32767];
    DWORD length = GetEnvironmentVariableW(L"Path", currentPath, 32767);

    std::wstring newPath;
    std::wstring searchPath = pathToRemove + L';';
    std::wstring currentPathStr = currentPath;

    size_t pos;
    while((pos = currentPathStr.find(searchPath)) != std::wstring::npos)
        currentPathStr.erase(pos, searchPath.length());

    if(!currentPathStr.empty() && currentPathStr.back() == L';')
        currentPathStr.pop_back();

    return SetEnvironmentVariablePersistent(L"Path", currentPathStr, true);
}

std::wstring GetInstallBase(bool isBin) {
    wchar_t path[MAX_PATH];

    if(isBin) {
        if(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILES, NULL, 0, path)))
            return std::wstring(path) + L"\\n8lang";
    }
    else {
        if(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
            return std::wstring(path) + L"\\n8lang";
    }

    return L"";
}

bool RemoveFileAssociation() {
    LSTATUS status;

    status = RegDeleteTreeW(HKEY_CLASSES_ROOT, L".n8");
    if(status != ERROR_SUCCESS && status != ERROR_FILE_NOT_FOUND)
        return false;

    status = RegDeleteTreeW(HKEY_CLASSES_ROOT, L"n8lang_script");
    if(status != ERROR_SUCCESS && status != ERROR_FILE_NOT_FOUND)
        return false;

    return true;
}

bool RemoveUninstallEntry() {
    std::wstring uninstallPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\N8Lang";
    LSTATUS status = RegDeleteTreeW(
        HKEY_LOCAL_MACHINE,
        uninstallPath.c_str()
    );

    return status == ERROR_SUCCESS || status == ERROR_FILE_NOT_FOUND;
}

int main() {
    if(!IsElevated()) {
        wchar_t path[MAX_PATH];

        GetModuleFileNameW(NULL, path, MAX_PATH);
        ShellExecuteW(NULL, L"runas", path, NULL, NULL, SW_SHOWNORMAL);

        return 0;
    }

    SetColor(Aqua);
    std::wcout << L"=== n8lang Uninstaller ===" << std::endl;
    ResetColor();

    std::wstring binPath = GetInstallBase(true);
    std::wstring docPath = GetInstallBase(false);

    if(!binPath.empty()) {
        SetColor(Yellow);
        std::wcout << L"Removing program files..." << std::endl;

        if(DeleteDirectory(binPath)) {
            SetColor(Green);
            std::wcout << L"Successfully removed: " << binPath << std::endl;
        }
        else {
            DWORD err = GetLastError();
            SetColor(Red);

            std::wcerr << L"Failed to remove: " << binPath 
                << L" (Error: " << err << L")" << std::endl;
        }

        ResetColor();
    }

    if(!docPath.empty()) {
        SetColor(Yellow);
        std::wcout << L"Removing user files..." << std::endl;
        
        if(DeleteDirectory(docPath)) {
            SetColor(Green);
            std::wcout << L"Successfully removed: " << docPath << std::endl;
        }
        else {
            DWORD err = GetLastError();
            SetColor(Red);

            std::wcerr << L"Failed to remove: " << docPath 
                << L" (Error: " << err << L")" << std::endl;
        }

        ResetColor();
    }

    SetColor(Yellow);
    std::wcout << L"Cleaning environment variables..." << std::endl;
    ResetColor();

    if (RemoveEnvironmentVariablePersistent(L"N8_PATH", false)) {
        SetColor(Green);
        std::wcout << L"Removed N8_PATH environment variable" << std::endl;
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to remove N8_PATH" << std::endl;
    }
    ResetColor();

    std::wstring binDirPath = GetInstallBase(true) + L"\\bin";
    if(RemoveFromSystemPath(binDirPath)) {
        SetColor(Green);
        std::wcout << L"Removed from PATH: "
            << binDirPath << std::endl;
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to remove from PATH" << std::endl;
    }
    ResetColor();

    SetColor(Yellow);
    std::wcout << L"Removing registry entries..." << std::endl;
    ResetColor();

    if(RemoveUninstallEntry()) {
        SetColor(Green);
        std::wcout << L"Removed Add/Remove Programs entry" << std::endl;
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to remove uninstall registry entry" << std::endl;
    }
    ResetColor();

    SetColor(Yellow);
    std::wcout << L"Removing file associations..." << std::endl;
    ResetColor();

    if(RemoveFileAssociation()) {
        SetColor(Green);
        std::wcout << L"Removed .n8 file associations" << std::endl;
    }
    else {
        SetColor(Red);
        std::wcerr << L"Failed to remove file associations" << std::endl;
    }
    ResetColor();

    SetColor(Aqua);
    std::wcout << L"\nUninstallation complete!" << std::endl;
    ResetColor();

    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    wchar_t tempDir[MAX_PATH];
    GetTempPathW(MAX_PATH, tempDir);

    wchar_t batPath[MAX_PATH];
    srand(static_cast<unsigned int>(time(NULL)));
    
    do swprintf_s(
        batPath,
        MAX_PATH,
        L"%s\\n8d_%04x.bat",
        tempDir,
        rand() % 0xFFFF
    );
    while(PathFileExistsW(batPath));

    HANDLE hBatFile = CreateFileW(
        batPath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if(hBatFile != INVALID_HANDLE_VALUE) {
        std::wstring batContent = 
            L"@echo off\r\n"
            L"timeout /t 1 /nobreak >nul\r\n"
            L"del \"" + std::wstring(exePath) + L"\"\r\n"
            L"del \"%~f0\"\r\n";
        
        DWORD bytesWritten;
        WriteFile(
            hBatFile,
            batContent.c_str(),
            static_cast<DWORD>(batContent.size() * sizeof(wchar_t)),
            &bytesWritten,
            NULL
        );
        CloseHandle(hBatFile);

        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        std::wstring cmdLine = L"cmd.exe /C \"" + std::wstring(batPath) + L"\"";
        if(CreateProcessW(
            NULL,
            &cmdLine[0],
            NULL,
            NULL,
            FALSE,
            CREATE_NO_WINDOW,
            NULL,
            NULL,
            &si,
            &pi
        )) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            SetColor(Red);
            std::wcerr << L"Failed to self-destruct" << std::endl;
            ResetColor();
        }
    }

    getchar();
    return 0;
}
