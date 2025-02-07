#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#include <gtkmm.h>
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>

namespace N8Uninstaller {

bool IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;

    if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);

        if(GetTokenInformation(
            hToken,
            TokenElevation,
            &Elevation,
            sizeof(Elevation),
            &cbSize
        )) fRet = Elevation.TokenIsElevated;
    }

    if(hToken)
        CloseHandle(hToken);

    return fRet;
}

bool SetEnvironmentVariablePersistent(
    const std::wstring& name,
    const std::wstring& value,
    bool systemWide
) {
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
    LSTATUS status = RegDeleteTreeW(HKEY_LOCAL_MACHINE, uninstallPath.c_str());

    return status == ERROR_SUCCESS || status == ERROR_FILE_NOT_FOUND;
}

}

class UninstallWindow : public Gtk::Window {
public:
    UninstallWindow();
    virtual ~UninstallWindow();

protected:
    Gtk::Box mainBox{Gtk::ORIENTATION_VERTICAL};
    Gtk::ScrolledWindow logScroll;
    Gtk::TextView logView;
    Gtk::ProgressBar progressBar;
    Gtk::Label statusLabel;
    Gtk::Button closeButton{"Close"};

    Glib::RefPtr<Gtk::TextBuffer> logBuffer;

    std::mutex logMutex;
    std::thread workerThread;

    void append_log(const std::string& message);
    void update_progress(double fraction);
    void update_status(const std::string& text);
    void on_close_clicked();
    void perform_uninstall();
    void create_selfdestruct_bat();
};

UninstallWindow::UninstallWindow() {
    this->set_title("N8 Language Uninstaller");
    this->set_position(Gtk::WIN_POS_CENTER);
    this->set_default_size(760, 380);
    this->set_resizable(false);
    this->set_keep_above(true);
    this->set_icon(Glib::RefPtr<Gdk::Pixbuf>());
    this->set_default_icon(Glib::RefPtr<Gdk::Pixbuf>());

    auto css = Gtk::CssProvider::create();
    css->load_from_data(R"(
        * {
            font-family: -apple-system, 'Helvetica', 'Segoe UI', sans-serif;
        }

        GtkTextView {
            font-family: Consolas;
        }

        .button {
            padding: 8px 20px;
            margin: 8px 0;
            font-weight: 500;
            transition: all 0.2s cubic-bezier(0.25, 0.46, 0.45, 0.94);
        }

        .log-view {
            border: 1px solid #909090;
        }

        .card {
            box-shadow: 0 8px 24px rgba(0,0,0,0.2);
            border: 1px solid #909090;
            padding: 24px;
            margin: 20px;
        }
    )");

    this->get_style_context()->add_provider_for_screen(
        Gdk::Screen::get_default(),
        css,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    Pango::FontDescription font;
    font.set_family("Consolas");

    this->mainBox.set_border_width(24);
    this->mainBox.set_spacing(16);
    this->mainBox.get_style_context()->add_class("card");
    this->add(this->mainBox);

    auto header = Gtk::make_managed<Gtk::Label>();
    header->set_markup("<span size='20pt' weight='bold'>N8 Language Uninstaller</span>");
    header->set_halign(Gtk::ALIGN_START);
    this->mainBox.pack_start(*header, Gtk::PACK_SHRINK);

    this->logBuffer = Gtk::TextBuffer::create();
    this->logView.set_buffer(logBuffer);
    this->logView.set_editable(false);
    this->logView.set_wrap_mode(Gtk::WRAP_WORD);
    this->logView.override_font(font);
    
    this->logScroll.add(this->logView);
    this->logScroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    this->logScroll.set_vexpand(true);
    this->logScroll.set_hexpand(true);
    this->logScroll.get_style_context()->add_class("log-view");
    this->logScroll.set_size_request(-1, 300);
    this->logScroll.set_overlay_scrolling(false);
    this->logScroll.set_kinetic_scrolling(true);
    this->mainBox.pack_start(this->logScroll);

    this->progressBar.set_show_text(true);
    this->progressBar.set_text("0%");
    this->progressBar.get_style_context()->add_class("progress");
    this->mainBox.pack_start(this->progressBar, Gtk::PACK_SHRINK);

    this->statusLabel.set_halign(Gtk::ALIGN_START);
    this->statusLabel.set_markup(
        "<span style='italic'>Initializing uninstallation...</span>"
    );
    this->mainBox.pack_start(this->statusLabel, Gtk::PACK_SHRINK);

    this->closeButton.set_sensitive(false);
    this->closeButton.get_style_context()->add_class("button");
    this->closeButton.signal_clicked()
        .connect(sigc::mem_fun(
            *this,
            &UninstallWindow::on_close_clicked
        ));
    this->mainBox.pack_start(this->closeButton, Gtk::PACK_SHRINK);

    this->show_all_children();
    this->workerThread = std::thread(&UninstallWindow::perform_uninstall, this);
}

UninstallWindow::~UninstallWindow() {
    if(this->workerThread.joinable())
        this->workerThread.join();
}

void UninstallWindow::append_log(const std::string& message) {
    this->logBuffer->insert(
        this->logBuffer->end(),
        message + "\n"
    );
}

void UninstallWindow::update_progress(double fraction) {
    Glib::signal_idle().connect_once([this, fraction]() {
        progressBar.set_fraction(fraction);
        progressBar.set_text(Glib::ustring::format(
            std::fixed, std::setprecision(0), fraction * 100, "%"
        ));
    });
}

void UninstallWindow::update_status(const std::string& text) {
    Glib::signal_idle().connect_once([this, text]() {
        statusLabel.set_markup(text);
    });
}

void UninstallWindow::on_close_clicked() {
    this->create_selfdestruct_bat();
    this->close();
}

void UninstallWindow::perform_uninstall() {
    this->append_log("Starting N8 Language uninstallation...");
    this->update_status("Initializing...");
    this->update_progress(0.0);

    std::wstring binPath = N8Uninstaller::GetInstallBase(true);
    if(!binPath.empty()) {
        this->update_status("Removing program files...");
        this->append_log("Removing program files...");

        if(N8Uninstaller::DeleteDirectory(binPath)) {
            this->append_log("Successfully removed program files!");
            this->update_progress(0.2);
        }
        else this->append_log("Failed to remove program files!");
    }

    std::wstring docPath = N8Uninstaller::GetInstallBase(false);
    if(!docPath.empty()) {
        this->update_status("Removing user files...");
        this->append_log("Removing user files...");

        if(N8Uninstaller::DeleteDirectory(docPath)) {
            this->append_log("Successfully removed user files!");
            this->update_progress(0.4);
        }
        else this->append_log("Failed to remove user files!");
    }

    this->update_status("Cleaning environment...");
    this->append_log("Removing N8_PATH...");

    if(N8Uninstaller::RemoveEnvironmentVariablePersistent(
        L"N8_PATH", false
    )) {
        this->append_log("Removed N8_PATH environment variable!");
        this->update_progress(0.6);
    }
    else this->append_log("Failed to remove N8_PATH!");

    std::wstring binDirPath = N8Uninstaller::GetInstallBase(true) +
        L"\\bin";
    this->append_log("Removing from system PATH...");

    if(N8Uninstaller::RemoveFromSystemPath(binDirPath)) {
        this->append_log("Removed from system PATH!");
        this->update_progress(0.7);
    }
    else this->append_log("Failed to remove from PATH!");

    this->update_status("Cleaning registry...");
    this->append_log("Removing uninstall entry...");

    if(N8Uninstaller::RemoveUninstallEntry()) {
        this->append_log("Removed registry entries!");
        this->update_progress(0.8);
    }
    else this->append_log("Failed to remove registry entries!");

    this->update_status("Removing file associations...");
    this->append_log("Removing .n8 associations...");

    if(N8Uninstaller::RemoveFileAssociation()) {
        this->append_log("Removed file associations!");
        this->update_progress(0.9);
    }
    else this->append_log("Failed to remove file associations!");

    this->create_selfdestruct_bat();
    this->update_progress(1.0);
    this->update_status("Uninstallation complete!");
    this->append_log("Uninstallation completed successfully!");

    Glib::signal_idle().connect_once([this]() {
        closeButton.set_sensitive(true);
    });
}

void UninstallWindow::create_selfdestruct_bat() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    wchar_t binDir[MAX_PATH];
    wcscpy_s(binDir, exePath);
    PathRemoveFileSpecW(binDir);

    wchar_t installDir[MAX_PATH];
    wcscpy_s(installDir, binDir);
    PathRemoveFileSpecW(installDir);

    wchar_t tempDir[MAX_PATH];
    GetTempPathW(MAX_PATH, tempDir);

    wchar_t batPath[MAX_PATH];
    do swprintf_s(
        batPath,
        L"%s\\n8d_%04x.bat",
        tempDir,
        rand() % 0xFFFF
    );
    while(PathFileExistsW(batPath));

    std::wstring batContentW =
        L"@echo off\r\n"
        L"timeout /t 5 /nobreak >nul\r\n"
        L":retry_exe\r\n"
        L"del \"" + std::wstring(exePath) + L"\" >nul 2>&1\r\n"
        L"if exist \"" + std::wstring(exePath) + L"\" (\r\n"
        L"    timeout /t 1 /nobreak >nul\r\n"
        L"    goto retry_exe\r\n"
        L")\r\n"
        L":retry_install\r\n"
        L"rmdir /S /Q \"" + std::wstring(installDir) + L"\"\r\n"
        L"if exist \"" + std::wstring(installDir) + L"\" (\r\n"
        L"    timeout /t 1 /nobreak >nul\r\n"
        L"    goto retry_install\r\n"
        L")\r\n"
        L"del \"%~f0\" >nul 2>&1\r\n";

    int ansiSize = WideCharToMultiByte(
        CP_ACP, 0,
        batContentW.c_str(), -1,
        nullptr, 0,
        nullptr,
        nullptr
    );
    std::string batContentA(ansiSize, '\0');

    WideCharToMultiByte(
        CP_ACP, 0,
        batContentW.c_str(), -1,
        &batContentA[0],
        ansiSize,
        nullptr,
        nullptr
    );

    HANDLE hBatFile = CreateFileW(
        batPath,
        GENERIC_WRITE, 0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if(hBatFile != INVALID_HANDLE_VALUE) {
        DWORD written;

        WriteFile(
            hBatFile,
            batContentA.c_str(),
            static_cast<DWORD>(batContentA.size()),
            &written,
            nullptr
        );
        CloseHandle(hBatFile);

        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        std::wstring cmdLine = L"cmd.exe /C \"" +
            std::wstring(batPath) + L"\"";

        CreateProcessW(
            nullptr,
            &cmdLine[0],
            nullptr,
            nullptr,
            FALSE,
            CREATE_NO_WINDOW,
            nullptr,
            tempDir,
            &si, &pi
        );

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

int main(int argc, char* argv[]) {
    if(!N8Uninstaller::IsElevated()) {
        wchar_t path[MAX_PATH];

        GetModuleFileNameW(nullptr, path, MAX_PATH);
        ShellExecuteW(nullptr, L"runas", path, nullptr, nullptr, SW_SHOWNORMAL);

        return 0;
    }

    auto app = Gtk::Application::create(
        argc,
        argv,
        "org.n8lang.uninstaller"
    );
    UninstallWindow window;

    return app->run(window);
}
