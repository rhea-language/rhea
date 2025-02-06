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

#ifndef N8_INSTALLER_WINDOW_HPP
#define N8_INSTALLER_WINDOW_HPP

#include <thread>
#include <gtkmm.h>

class InstallerWindow : public Gtk::Window {
public:
    InstallerWindow();
    virtual ~InstallerWindow();

private:
    Gtk::Stack mainStack;
    Gtk::Box licenseBox{Gtk::ORIENTATION_VERTICAL};
    Gtk::Box installBox{Gtk::ORIENTATION_VERTICAL};
    Gtk::ScrolledWindow licenseScroll;
    Gtk::TextView licenseView;
    Gtk::Button agreeButton{"I Agree"};
    Gtk::Label headerLabel;
    Gtk::ScrolledWindow logScroll;
    Gtk::TextView logView;
    Gtk::ProgressBar progressBar;
    Gtk::Label statusLabel;
    Gtk::Button closeButton{"Close"};

    bool installSuccess = true;
    std::thread workerThread;
    Glib::RefPtr<Gtk::TextBuffer> logBuffer;
    Glib::RefPtr<Gtk::TextTag> tagSuccess, tagError, tagInfo;

    void setup_license_page();
    void setup_install_page();
    void on_license_scroll();
    void start_installation();

    void append_log(const std::string& message, const Glib::RefPtr<Gtk::TextTag>& tag);
    void update_progress(double fraction);
    void update_status(const std::string& status);
    void installation_done();

    bool extract_files();
    bool set_environment();
    bool add_to_path();
    bool create_associations();
    bool create_uninstaller();
    void run_installation();
};

#endif
