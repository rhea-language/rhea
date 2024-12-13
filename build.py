# Copyright (c) 2024 - Nathanne Isip
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

import cpuinfo
import os
import platform
import requests
import shutil
import subprocess
import zipfile

CORE_VERSION = '1.0.0'
OUT_DIR = os.path.join('dist', 'n8lang')
TEMP_DIR = 'temp'

if os.path.exists(OUT_DIR):
    shutil.rmtree(OUT_DIR)

if os.path.exists(TEMP_DIR):
    shutil.rmtree(TEMP_DIR)

os.makedirs(OUT_DIR)
os.makedirs(TEMP_DIR)

try:
    src = 'modules'

    shutil.copytree(src, os.path.join(OUT_DIR, os.path.basename(src)), dirs_exist_ok=True)
    print(f"Successfully copied '{src}' to '{OUT_DIR}'")

except Exception as e:
    print("Failed to copy modules.")
    exit(0)

PLATFORM = platform.system()
ARCH = platform.architecture()[0]
OUTPUT_EXECUTABLE = os.path.join(OUT_DIR, 'bin')
OUTPUT_LIBRARY = os.path.join(OUT_DIR, 'modules', 'core@' + CORE_VERSION, 'lib')

os.makedirs(OUTPUT_EXECUTABLE)
os.makedirs(OUTPUT_LIBRARY)

OUTPUT_EXECUTABLE = os.path.join(OUTPUT_EXECUTABLE, 'n8')
OUTPUT_LIBRARY = os.path.join(OUTPUT_LIBRARY, 'n8-std')

cpp_files = []
cc_files = []

lib_headers = [
    '-Ilib/glfw/include',
    '-Ilib/QuickDigest5/include',
    '-Ilib/MyShell/include',
    '-Ilib/SHA/src'
]
lib_source_files = []

try:
    print('Pulling submodules...', end='')
    subprocess.run(['git', 'submodule', 'update', '--init', '--recursive'])
    subprocess.run(['git', 'pull', '--recurse-submodules'])

except Exception as e:
    print(f"Failed to pull up submodules with error: {e}")
    exit(1)

def get_ext_instructions():
    print('Checking extended instruction availability...')
    features_to_check = [
        "abm", "adx", "aes", "avx", "avx2", "bmi",
        "clflushopt", "cx16", "f16c", "fma", "fsgsbase",
        "fxsr", "mmx", "movbe", "rdrnd", "rdseed",
        "rtm", "sgx", "sse", "sse2", "sse3", "sse4",
        "sse4_1", "sse4_2", "vpclmulqdq", "xsave",
        "xsavec", "xsaveopt", "xsave"
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

def get_glfw_file(arch):
    url = "https://github.com/glfw/glfw/releases/download/3.4/"

    if arch == 'win32':
        return url + 'glfw-3.4.bin.WIN32.zip'
    elif arch == 'win64':
        return url + 'glfw-3.4.bin.WIN64.zip'
    elif arch == 'darwin':
        return url + 'glfw-3.4.bin.MACOS.zip'

    raise ValueError('Invalid architecture for GLFW')

def download_file(url, local_filename):
    try:
        response = requests.get(url, stream=True)
        response.raise_for_status()

        local_filename = os.path.join(TEMP_DIR, local_filename)
        with open(local_filename, 'wb') as f:
            for chunk in response.iter_content(chunk_size=8192):
                if chunk:
                    f.write(chunk)

        print(f"{local_filename} has been downloaded successfully.")
        unzip_and_move_contents(local_filename, TEMP_DIR)

    except requests.RequestException as e:
        print(f"Failed to download {url}. Error: {e}")

def unzip_and_move_contents(zip_file, destination):
    if not os.path.exists(destination):
        os.makedirs(destination)

    with zipfile.ZipFile(zip_file, 'r') as zip_ref:
        zip_ref.extractall(destination)

    print(f"Contents of {zip_file} have been extracted to {destination}")

def include_sha_headers():
    global lib_source_files

    for root, _, files in os.walk('lib/SHA/src'):
        for file in files:
            if file.endswith('.cpp'):
                lib_source_files.append(os.path.join(root, file))

    lib_source_files = [file for file in lib_source_files if not file.endswith("example.cpp")]

for root, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.cpp'):
            cpp_files.append(os.path.join(root, file))

for root, dirs, files in os.walk('std'):
    for file in files:
        if file.endswith('.cc'):
            cc_files.append(os.path.join(root, file))

for root, dirs, files in os.walk('lib/QuickDigest5/src'):
    for file in files:
        if file.endswith('.cpp'):
            lib_source_files.append(os.path.join(root, file))

for root, dirs, files in os.walk('lib/MyShell/src'):
    for file in files:
        if file.endswith('.cpp'):
            lib_source_files.append(os.path.join(root, file))

try:
    ext_instructions = get_ext_instructions()
    if PLATFORM == 'Linux':
        subprocess.run([
            'sudo', 'apt', 'install',
            '-y', 'libglfw3-dev', 'libgl1-mesa-dev'
        ])

    print('Building binaries...')
    if PLATFORM == 'Windows':
        if ARCH == '64bit':
            download_file(get_glfw_file('win64'), 'glfw.zip')
            shutil.move(
                os.path.join(TEMP_DIR, 'glfw-3.4.bin.WIN64'),
                os.path.join(TEMP_DIR, 'glfw-3.4')
            )
        elif ARCH == '32bit':
            download_file(get_glfw_file('win32'), 'glfw.zip')
            shutil.move(
                os.path.join(TEMP_DIR, 'glfw-3.4.bin.WIN32'),
                os.path.join(TEMP_DIR, 'glfw-3.4')
            )

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
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        glfw_lib_path = os.path.join(TEMP_DIR, 'glfw-3.4', 'lib-mingw-w64')
        lib_source_files += [
            os.path.join(glfw_lib_path, 'libglfw3.a'),
            os.path.join(glfw_lib_path, 'libglfw3dll.a'),
            os.path.join(glfw_lib_path, 'glfw3.dll')
        ]
        lib_build_args = [
            'g++', '-static', '-static-libgcc', '-Iinclude',
            '-Istd', '-shared', '-o', OUTPUT_LIBRARY + '.dll',
            '-L' + os.path.join(TEMP_DIR, 'glfw-3.4', 'lib-mingw-w64')
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

    elif "TERMUX_VERSION" in os.environ:
        ext_instructions.remove('-maes')
        exe_build_args= [
            'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
            '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
            '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror',
            '-Wfloat-equal', '-Wformat', '-Wformat=2', '-Wformat-nonliteral',
            '-Wformat-security', '-Wformat-y2k', '-Wimport', '-Winit-self',
            '-Winvalid-pch', '-Wno-header-guard', '-Wlong-long',
            '-Wmissing-braces', '-Wmissing-field-initializers', '-Wmissing-format-attribute',
            '-Wmissing-include-dirs', '-Weffc++', '-Wpacked', '-Wparentheses',
            '-Wpointer-arith', '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
            '-Wshadow', '-Wsign-compare', '-Wstack-protector', '-Wstrict-aliasing',
            '-Wstrict-aliasing=2', '-Wswitch', '-Wswitch-default', '-Wswitch-enum',
            '-Wtrigraphs', '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
            '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros', '-O3',
            '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-ffast-math', '-s',
            '-std=c++20', '-fopenmp'] + ext_instructions + ['-march=native',
            '-funroll-loops', '-ffast-math', '-D__TERMUX__'
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        lib_build_args = [
            'g++', '-Iinclude', '-Istd', '-fPIC',
            '-shared', '-o', OUTPUT_LIBRARY + '.so'
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

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
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros', '-O3',
            '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-ffast-math', '-s',
            '-std=c++20', '-fopenmp'] + ext_instructions + ['-mfpmath=sse',
            '-march=native', '-funroll-loops', '-ffast-math'
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        lib_build_args = [
            'g++', '-Iinclude', '-Istd', '-fPIC',
            '-shared', '-o', OUTPUT_LIBRARY + '.so'
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

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
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        lib_build_args = [
            '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude',
            '-Istd', '-shared', '-o', OUTPUT_LIBRARY + '.dylib'
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

except Exception as e:
    print(f"Compilation failed with error: {e}")
    exit(1)
