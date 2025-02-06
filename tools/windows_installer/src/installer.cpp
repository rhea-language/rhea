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

#include <installer_window.hpp>
#include <installer_util.hpp>

#include <windows.h>

int main(int argc, char* argv[]) {
    if(!N8InstallerUtil::IsElevated()) {
        wchar_t path[MAX_PATH];

        GetModuleFileNameW(NULL, path, MAX_PATH);
        ShellExecuteW(NULL, L"runas", path, NULL, NULL, SW_SHOWNORMAL);

        return 0;
    }

    auto app = Gtk::Application::create(argc, argv, "org.n8lang.installer");
    InstallerWindow window;

    window.signal_hide().connect([&app] {
        app->quit();
    });
    return app->run(window);
}
