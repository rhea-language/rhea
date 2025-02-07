# Copyright (c) 2025 - Nathanne Isip
# This file is part of N8.
# 
# N8 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
# 
# N8 is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with N8. If not, see <https://www.gnu.org/licenses/>.

import os
import platform
import subprocess
import sys

from datetime import datetime

def log_info(message):
    print(
        "\033[1m[\033[0m \033[1;42m  INFO   \033[0m \033[1m]\033[0m \033[3m" +
        str(datetime.today()) + "\033[0m :: " + message
    )

def log_task(message):
    print(
        "\033[1m[\033[0m \033[1;44m  TASK   \033[0m \033[1m]\033[0m \033[3m" +
        str(datetime.today()) + "\033[0m :: " + message
    )

def log_warning(message):
    print(
        "\033[1m[\033[0m \033[1;43m WARNING \033[0m \033[1m]\033[0m \033[3m" +
        str(datetime.today()) + "\033[0m :: " + message
    )

def log_error(message):
    print(
        "\033[1m[\033[0m \033[1;41m  ERROR  \033[0m \033[1m]\033[0m \033[3m" +
        str(datetime.today()) + "\033[0m :: " + message
    )

def build_proc():
    msys2_path = 'D:\\a\\_temp\\msys64\\mingw64\\'
    if len(sys.argv) == 2:
        msys2_path = sys.argv[1]
    log_info("Using msys2 path at " + msys2_path)

    VERSION         = '1.0.0'
    ARCHITECTURE    = platform.machine().lower()
    OUTPUT_EXE      = os.path.join(
        'dist', 'n8lang', 'bin',
        f'uninstaller.exe'
    )

    config_res = 'dist\\n8-uninstaller-config.res'
    icon_config_res = 'dist\\n8-uninstaller-icon-config.res'

    log_task("Generating Windows resource file configurations...")
    subprocess.run(['windres', 'configs\\n8-uninstaller-config.rc', '-O', 'coff', '-o', config_res])
    subprocess.run(['windres', 'configs\\n8-uninstaller-icon-config.rc', '-O', 'coff', '-o', icon_config_res])
    log_info("Windows resource file configurations successfully generated!")

    log_task("Executing build command subprocess...")
    subprocess.run([
        'g++', '-o', OUTPUT_EXE,
        '-DLIBDEFLATE_DLL', '-mwindows',

        'tools\\windows_uninstaller\\uninstaller.cpp',

        config_res,
        icon_config_res,

        '-I' + os.path.join(msys2_path, 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'gtkmm-3.0'),
        '-I' + os.path.join(msys2_path, 'lib', 'gtkmm-3.0', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'atkmm-1.6'),
        '-I' + os.path.join(msys2_path, 'lib', 'atkmm-1.6', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'gdkmm-3.0'),
        '-I' + os.path.join(msys2_path, 'lib', 'gdkmm-3.0', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'giomm-2.4'),
        '-I' + os.path.join(msys2_path, 'lib', 'giomm-2.4', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'gtk-3.0'),
        '-I' + os.path.join(msys2_path, 'include', 'cairo'),
        '-I' + os.path.join(msys2_path, 'include', 'atk-1.0'),
        '-I' + os.path.join(msys2_path, 'include', 'pangomm-1.4'),
        '-I' + os.path.join(msys2_path, 'lib', 'pangomm-1.4', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'glibmm-2.4'),
        '-I' + os.path.join(msys2_path, 'lib', 'glibmm-2.4', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'cairomm-1.0'),
        '-I' + os.path.join(msys2_path, 'lib', 'cairomm-1.0', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'sigc++-2.0'),
        '-I' + os.path.join(msys2_path, 'lib', 'sigc++-2.0', 'include'),
        '-I' + os.path.join(msys2_path, 'include', 'pango-1.0'),
        '-I' + os.path.join(msys2_path, 'include', 'pixman-1'),
        '-I' + os.path.join(msys2_path, 'include', 'fribidi'),
        '-I' + os.path.join(msys2_path, 'include', 'harfbuzz'),
        '-I' + os.path.join(msys2_path, 'include', 'freetype2'),
        '-I' + os.path.join(msys2_path, 'include', 'gdk-pixbuf-2.0'),
        '-I' + os.path.join(msys2_path, 'include', 'libpng16'),
        '-I' + os.path.join(msys2_path, 'include', 'webp'),
        '-I' + os.path.join(msys2_path, 'include', 'glib-2.0'),
        '-I' + os.path.join(msys2_path, 'lib', 'glib-2.0', 'include'),

        '-L' + os.path.join(msys2_path, 'lib'),
        '-L' + os.path.join(msys2_path, 'bin'),

        '-lgtkmm-3.0', '-latkmm-1.6', '-lgdkmm-3.0',
        '-lgdk-3', '-lz', '-lgdi32', '-limm32', '-lshell32',
        '-lole32', '-luuid', '-lwinmm', '-ldwmapi',
        '-lsetupapi', '-lhid', '-lwinspool', '-lcomctl32',
        '-lcomdlg32', '-lcairo-gobject', '-latk-1.0',
        '-lpangomm-1.4', '-lglibmm-2.4', '-lcairomm-1.0',
        '-lsigc-2.0', '-lpangocairo-1.0', '-lpangowin32-1.0',
        '-lpango-1.0', '-lcairo', '-lharfbuzz',
        '-lgdk_pixbuf-2.0', '-lgio-2.0', '-lgobject-2.0',
        '-lglib-2.0', '-lintl', '-lgiomm-2.4',
        '-lshlwapi', '-ladvapi32', '-luser32'
    ])
    log_info("Windows GUI uninstaller build process done!")

    log_warning("Cleaning up generated Windows resource file configurations...")
    os.remove(config_res)
    os.remove(icon_config_res)
    log_info("Clean up done!")

    if os.path.exists(OUTPUT_EXE):
        log_info("Executable file generated at " + OUTPUT_EXE)
    else:
        log_error("No executable file was generated.")

try:
    build_proc()
except KeyboardInterrupt:
    log_error("Process interrupted. Exiting...")
