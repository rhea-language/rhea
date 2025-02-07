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

def file_to_c_array(file_path, array_name):
    with open(file_path, "rb") as f:
        data = f.read()
    hex_data = ", ".join(f"0x{byte:02X}" for byte in data)
    return f"const unsigned char {array_name}[] = {{ {hex_data} }};"

def build_proc():
    msys2_path = 'D:\\a\\_temp\\msys64\\mingw64\\'
    if len(sys.argv) == 2:
        msys2_path = sys.argv[1]
    log_info("Using msys2 path at " + msys2_path)

    files = {
        "dist\\n8lang\\bin\\n8.exe":                            "n8_exe",
        "dist\\n8lang\\bin\\uninstaller.exe":                   "uninstaller",

        "dist\\n8lang\\modules\\core@1.0.0\\lib\\core.dll":     "core_dll",

        "dist\\n8lang\\modules\\core@1.0.0\\src\\array.n8":     "array_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\chrono.n8":    "chrono_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\crypt.n8":     "crypt_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\env.n8":       "env_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\gl.n8":        "gl_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\io.n8":        "io_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\lang.n8":      "lang_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\math.n8":      "math_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\ml.n8":        "ml_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\reflect.n8":   "reflect_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\str.n8":       "str_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\sys.n8":       "sys_n8",
        "dist\\n8lang\\modules\\core@1.0.0\\src\\unsafe.n8":    "unsafe_n8",

        "dist\\n8lang\\modules\\core@1.0.0\\config.sconf":      "config_sconf"
    }

    embedded_files = "tools\\windows_installer\\include\\installer_embedded_files.hpp"
    embedded_file_map = "tools\\windows_installer\\include\\installer_embedded_file_map.hpp"

    try:
        log_warning("Cleaning previously generated embeddable files and maps...")
        if os.path.exists(embedded_files):
            os.remove(embedded_files)

        if os.path.exists(embedded_file_map):
            os.remove(embedded_file_map)
        
        log_info("Clean-up done!")

    except(e):
        log_error("Cannot delete meta-generated files.")

    log_task("Generating embedded files content mapping...")
    with open(embedded_files, "w") as f:
        f.write("#ifndef EMBEDDED_FILES_H\n#define N8_INSTALLER_EMBEDDED_FILES_H\n\n")
        for file_name, array_name in files.items():
            f.write(file_to_c_array(file_name, array_name) + "\n\n")
        f.write("#endif // EMBEDDED_FILES_H\n")
    log_info("Embedded files content mapping successfully generated!")

    log_task("Generating embedded files map...")
    with open(embedded_file_map, "w") as f:
        f.write("const FileMapping files[] = {\n")

        total_items = len(files)
        for i, (file_name, array_name) in enumerate(files.items()):
            f.write(
                "    { \"" + array_name +
                "\", " + array_name +
                ", sizeof(" + array_name +
                "), L\"" + file_name.replace("\\", "\\\\") +
                "\" }"
            )

            if i < total_items - 1:
                f.write(",\n")
            else:
                f.write("\n")

        f.write("};\n")
    log_info("Embedded files map successfully generated!")

    VERSION         = '1.0.0'
    ARCHITECTURE    = platform.machine().lower()
    OUTPUT_EXE      = os.path.join(
        'dist',
        f'n8-lang_{VERSION}_{ARCHITECTURE}.exe'
    )

    log_task("Executing build command subprocess...")
    subprocess.run([
        'g++', '-o', OUTPUT_EXE,
        '-DLIBDEFLATE_DLL', '-mwindows',

        'tools\\windows_installer\\src\\installer_window.cpp',
        'tools\\windows_installer\\src\\installer_util.cpp',
        'tools\\windows_installer\\src\\installer.cpp',

        '-I' + os.path.join('tools', 'windows_installer', 'include'),
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
        '-lglib-2.0', '-lintl', '-lgiomm-2.4'
    ])
    log_info("Windows GUI installer build process done!")

    if os.path.exists(OUTPUT_EXE):
        log_info("Executable file generated at " + OUTPUT_EXE)
    else:
        log_error("No executable file was generated.")

try:
    build_proc()
except KeyboardInterrupt:
    log_error("Process interrupted. Exiting...")
