# Copyright (c) 2024 - Nathanne Isip
# This file is part of Zhivo.
# 
# Zhivo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
# 
# Zhivo is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Zhivo. If not, see <https://www.gnu.org/licenses/>.

import cpuinfo
import os
import platform
import requests
import shutil
import subprocess
import sys
import tarfile
import zipfile

from glob import glob

PLATFORM = platform.system()

OUT_DIR = 'dist'
if os.path.exists(OUT_DIR):
    shutil.rmtree(OUT_DIR)
os.makedirs(OUT_DIR)

TEMP_DIR = 'temp'
if os.path.exists(TEMP_DIR):
    shutil.rmtree(TEMP_DIR)
os.makedirs(TEMP_DIR)

OUTPUT_EXECUTABLE = os.path.join(OUT_DIR, 'zhivo')
OUTPUT_LIBRARY = os.path.join(OUT_DIR, 'zhvstdlib')
OUTPUT_CORE = OUTPUT_LIBRARY + '-core.a'

cpp_files = []
cc_files = []

def get_ext_instructions():
    print('Checking extended instruction availability...')
    features_to_check = [
        "mmx", "sse", "sse2", "sse3",
        "sse4", "sse4_1", "sse4_2",
        "avx", "avx2", "fma"
    ]

    supported_features = []
    cpu_info = cpuinfo.get_cpu_info()

    if 'flags' in cpu_info:
        for feature in features_to_check:
            print('Checking extended instruction support for ' + feature + '... ', end='')
            if feature in cpu_info['flags']:
                supported_features.append('-m' + feature.replace('_', '.'))
                print('supported')
            else:
                print('not found')

    return supported_features

def download_libui():
    url = ''
    if PLATFORM == 'Windows':
        url = 'https://github.com/andlabs/libui/releases/download/alpha4.1/libui-alpha4.1-windows-amd64-shared.zip'
    elif PLATFORM == 'Linux':
        url = 'https://github.com/andlabs/libui/releases/download/alpha4.1/libui-alpha4.1-linux-amd64-shared.tgz'
    elif PLATFORM == 'Darwin':
        url = 'https://github.com/andlabs/libui/releases/download/alpha4.1/libui-alpha4.1-darwin-amd64-shared.tgz'

    zip_path = url.split('/')[-1]

    print('Downloading libui release...')
    response = requests.get(url)
    with open(zip_path, 'wb') as file:
        file.write(response.content)
    print('Download completed.')
    os.makedirs(OUT_DIR, exist_ok=True)

    if zip_path.endswith('.zip'):
        print('Extracting the ZIP file...')
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall(OUT_DIR)
        print('Extraction completed.')
    elif zip_path.endswith('.tgz') or zip_path.endswith('.tar.gz'):
        print('Extracting the TAR file...')
        with tarfile.open(zip_path, 'r:gz') as tar_ref:
            tar_ref.extractall(OUT_DIR)
        print('Extraction completed.')
    else:
        print('Unsupported file format for extraction.')

    lib_header_path = os.path.join(TEMP_DIR, 'include')
    os.makedirs(TEMP_DIR, exist_ok=True)
    os.makedirs(lib_header_path, exist_ok=True)

    header_files = glob(os.path.join(OUT_DIR, '*.h'))
    for header_file in header_files:
        shutil.move(header_file, lib_header_path)

    os.remove(zip_path)

for root, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.cpp'):
            cpp_files.append(os.path.join(root, file))

for root, dirs, files in os.walk('lib'):
    for file in files:
        if file.endswith('.cc'):
            cc_files.append(os.path.join(root, file))

try:
    ext_instructions = get_ext_instructions()
    download_libui()

    if PLATFORM == 'Windows':

        exe_build_args= [
            'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
            '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
            '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror',
            '-Wfloat-equal', '-Wformat', '-Wformat=2', '-Wformat-nonliteral',
            '-Wformat-security', '-Wformat-y2k', '-Wimport', '-Winit-self',
            '-Winvalid-pch', '-Wunsafe-loop-optimizations', '-Wlong-long',
            '-Wmissing-braces', '-Wmissing-field-initializers', '-Wmissing-format-attribute',
            '-Wmissing-include-dirs', '-Weffc++', '-Wpacked', '-Wparentheses',
            '-Wpointer-arith', '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
            '-Wshadow', '-Wsign-compare', '-Wstack-protector', '-Wstrict-aliasing',
            '-Wstrict-aliasing=2', '-Wswitch', '-Wswitch-default', '-Wswitch-enum',
            '-Wtrigraphs', '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
            '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros',
            '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-Ofast', '-s',
            '-std=c++17', '-fopenmp'] + ext_instructions + ['-mfpmath=sse',
            '-march=native', '-funroll-loops', '-ffast-math', '-static', '-static-libgcc',
            '-static-libstdc++'
        ] + cpp_files

        core_build_args = exe_build_args + [
            '-shared',
            '-o', OUTPUT_CORE
        ]
        exe_build_args += ['-o', OUTPUT_EXECUTABLE]
        lib_build_args = [
            'g++', '-static', '-static-libgcc', '-Iinclude',
            '-Ilib', '-I' + os.path.join(TEMP_DIR, 'include'),
            '-shared', '-o', OUTPUT_LIBRARY + '.dll', OUTPUT_CORE,
            os.path.join(OUT_DIR, 'libui.dll')
        ] + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(core_build_args))
        subprocess.run(core_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

    elif PLATFORM == 'Linux':
        exe_build_args= [
            'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
            '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
            '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror',
            '-Wfloat-equal', '-Wformat', '-Wformat=2', '-Wformat-nonliteral',
            '-Wformat-security', '-Wformat-y2k', '-Wimport', '-Winit-self',
            '-Winvalid-pch', '-Wunsafe-loop-optimizations', '-Wlong-long',
            '-Wmissing-braces', '-Wmissing-field-initializers', '-Wmissing-format-attribute',
            '-Wmissing-include-dirs', '-Weffc++', '-Wpacked', '-Wparentheses',
            '-Wpointer-arith', '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
            '-Wshadow', '-Wsign-compare', '-Wstack-protector', '-Wstrict-aliasing',
            '-Wstrict-aliasing=2', '-Wswitch', '-Wswitch-default', '-Wswitch-enum',
            '-Wtrigraphs', '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
            '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros',
            '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-Ofast', '-s',
            '-std=c++20', '-fopenmp'] + ext_instructions + ['-mfpmath=sse',
            '-march=native', '-funroll-loops', '-ffast-math'
        ] + cpp_files

        core_build_args = exe_build_args + [
            '-fPIC', '-shared',
            '-o', OUTPUT_CORE
        ]
        exe_build_args += ['-o', OUTPUT_EXECUTABLE]
        lib_build_args = [
            'g++', '-Iinclude',
            '-Ilib', '-I' + os.path.join(TEMP_DIR, 'include'),
            '-fPIC', '-shared', '-o', OUTPUT_LIBRARY + '.so',
            OUTPUT_CORE, os.path.join(OUT_DIR, 'libui.so')
        ] + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(core_build_args))
        subprocess.run(core_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

    elif PLATFORM == 'Darwin':
        exe_build_args = [
            '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude', '-Wall',
            '-pedantic', '-Wdisabled-optimization', '-pedantic-errors',
            '-Wextra', '-Wcast-align', '-Wcast-qual', '-Wchar-subscripts',
            '-Wcomment', '-Wconversion', '-Werror', '-Wfloat-equal',
            '-Wformat', '-Wformat=2', '-Wformat-nonliteral', '-Wformat-security',
            '-Wformat-y2k', '-Wimport', '-Winit-self', '-Winvalid-pch',
            '-Wlong-long', '-Wmissing-braces', '-Wmissing-field-initializers',
            '-Wmissing-format-attribute', '-Wmissing-include-dirs', '-Wpacked',
            '-Wparentheses', '-Wpointer-arith', '-Wredundant-decls',
            '-Wsequence-point', '-Wshadow', '-Wsign-compare', '-Wstack-protector',
            '-Wstrict-aliasing', '-Wstrict-aliasing=2', '-Wswitch',
            '-Wswitch-default', '-Wswitch-enum', '-Wtrigraphs', '-Wuninitialized',
            '-Wunknown-pragmas', '-Wunreachable-code', '-Wunused',
            '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros',
            '-Wwrite-strings', '-Wno-return-type-c-linkage', '-pipe',
            '-std=c++17', '-fopenmp', '-march=native', '-funroll-loops',
            '-ffast-math', '-flto=auto', '-Xpreprocessor', '-O3',
            '-Wno-header-guard', '-Wno-pessimizing-move'
        ] + cpp_files

        core_build_args = exe_build_args + ['-shared', '-o', OUTPUT_CORE]
        exe_build_args += ['-o', OUTPUT_EXECUTABLE]

        libui_dylib = (
            '' if PLATFORM == 'Darwin' and platform.machine() == 'arm64'
            else os.path.join(OUT_DIR, 'libui.dylib')
        )
        lib_build_args = [
            '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude',
            '-Ilib', '-I' + os.path.join(TEMP_DIR, 'include'),
            '-shared', '-o', OUTPUT_LIBRARY + '.dylib', OUTPUT_CORE,
            libui_dylib
        ] + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(core_build_args))
        subprocess.run(core_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

except Exception as e:
    print(f"Compilation failed with error: {e}")
    exit(1)
