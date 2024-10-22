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

import os
import platform
import subprocess
import sys

COMPILER = 'g++'
OUT_DIR = 'dist'

APP_ICON_PATH = os.path.join(OUT_DIR, 'app_icon.rc')
APP_ICON_OBJ = os.path.join(OUT_DIR, 'app_icon.o')

PLATFORM = platform.system()
OUTPUT_EXECUTABLE = os.path.join(
    'dist',
    'zhivo-' +
        sys.platform + '-' +
        platform.machine().lower()
)

cpp_files = []
cc_files = []

for root, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.cpp'):
            cpp_files.append(os.path.join(root, file))

for root, dirs, files in os.walk('lib'):
    for file in files:
        if file.endswith('.cc'):
            cc_files.append(os.path.join(root, file))

if not cpp_files:
    print("No .cpp files found in the src directory.")
    exit(1)

if PLATFORM == 'Darwin':
    COMPILER = '/opt/homebrew/opt/llvm/bin/clang++'

exe_build_args = [
    COMPILER, '-Iinclude',
    '-Wall', '-pedantic', '-Wdisabled-optimization',
    '-pedantic-errors', '-Wextra', '-Wcast-align',
    '-Wcast-qual', '-Wchar-subscripts', '-Wcomment',
    '-Wconversion', '-Werror', '-Wfloat-equal', '-Wformat',
    '-Wformat=2', '-Wformat-nonliteral', '-Wformat-security',
    '-Wformat-y2k', '-Wimport', '-Winit-self', '-Winvalid-pch',
    '-Wunsafe-loop-optimizations', '-Wlong-long',
    '-Wmissing-braces', '-Wmissing-field-initializers',
    '-Wmissing-format-attribute', '-Wmissing-include-dirs',
    '-Weffc++', '-Wpacked', '-Wparentheses', '-Wpointer-arith',
    '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
    '-Wshadow', '-Wsign-compare', '-Wstack-protector',
    '-Wstrict-aliasing', '-Wstrict-aliasing=2', '-Wswitch',
    '-Wswitch-default', '-Wswitch-enum', '-Wtrigraphs',
    '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
    '-Wunused', '-Wunused-function', '-Wunused-label',
    '-Wunused-parameter', '-Wunused-value', '-Wunused-variable',
    '-Wvariadic-macros', '-Wvolatile-register-var', '-Wwrite-strings',
    '-pipe', '-Ofast', '-s', '-std=c++17', '-fopenmp',
    '-msse', '-msse2', '-msse3', '-mfpmath=sse',
    '-march=native', '-funroll-loops', '-ffast-math'
]

if PLATFORM != 'Windows':
    exe_build_args.append('-flto=auto')

if PLATFORM == 'Darwin':
    exe_build_args.append('-Xpreprocessor')
    exe_build_args.append('-O3')
    exe_build_args.append('-Wno-header-guard')
    exe_build_args.append('-Wno-pessimizing-move')
    exe_build_args.remove('-Wunsafe-loop-optimizations')
    exe_build_args.remove('-Wvolatile-register-var')
    exe_build_args.remove('-Weffc++')
    exe_build_args.remove('-Ofast')
    exe_build_args.remove('-msse')
    exe_build_args.remove('-msse2')
    exe_build_args.remove('-msse3')
    exe_build_args.remove('-mfpmath=sse')
    exe_build_args.remove('-s')

rt_bin = os.path.join('dist', 'zhivocore-1.0.a')
rt_build_args = exe_build_args + cpp_files + [
    '-shared',
    '-o',
    rt_bin,
]

lib_bin = os.path.join('dist', 'stdzhv1.0')
if PLATFORM == 'Windows':
    lib_bin = lib_bin + ".dll"
else:
    lib_bin = lib_bin + ".so"

lib_build_args = [
    COMPILER, '-Iinclude',
    '-Ilib', '-shared', '-o',
    lib_bin, rt_bin
]

exe_build_args += cpp_files + ['-o', OUTPUT_EXECUTABLE + '-omp']
lib_build_args += cc_files
cuda_build_args = ['nvcc']

if PLATFORM == 'Linux':
    lib_build_args.append('-fPIC')

if PLATFORM == 'Windows':
    cuda_build_args.append('-Xcompiler')
    cuda_build_args.append('/std:c++17')

    lib_build_args = lib_build_args + [
        '-pipe', '-Ofast', '-s',
        '-std=c++17', '-fopenmp',
        '-msse', '-msse2', '-msse3',
        '-mfpmath=sse',
        '-march=native'
    ]

cuda_build_args.append('-Iinclude')
cuda_build_args += cpp_files + ['-o', OUTPUT_EXECUTABLE + '-nvidia']

cuda_lib_args = [
    'nvcc', '-Ilib', '-shared', '-o',
    os.path.join('dist', 'stdzhv1.0-nvidia'),
    rt_bin, cc_files
]

if PLATFORM == 'Windows':
    cuda_lib_args[4] += '.dll'
else:
    cuda_lib_args[4] += '.so'

try:
    os.makedirs(OUT_DIR, exist_ok=True)
    if PLATFORM == 'Windows':
        app_icon = open(APP_ICON_PATH, "w")
        app_icon.write('app_icon ICON "assets/zhivo-logo.ico"')
        app_icon.close()

        subprocess.run([
            'windres',
            APP_ICON_PATH,
            '-o',
            APP_ICON_OBJ
        ], check=True)

        exe_build_args.append(APP_ICON_OBJ)
        cuda_build_args.append(APP_ICON_OBJ)

    print("Executing:")
    print(' '.join(exe_build_args))
    subprocess.run(exe_build_args, check=True)

    print("Executing:")
    print(' '.join(rt_build_args))
    subprocess.run(rt_build_args, check=True)

    print("Executing:")
    print(' '.join(lib_build_args))
    subprocess.run(lib_build_args, check=True)

    if PLATFORM != 'Darwin':
        print("Executing:")
        print(' '.join(cuda_build_args))
        subprocess.run(cuda_build_args, check=True)

        print("Executing:")
        print(' '.join(cuda_lib_args))
        subprocess.run(cuda_lib_args, check=True)

except Exception as e:
    print(f"Compilation failed with error: {e}")

finally:
    print("Performing quick clean up...")

    if PLATFORM == 'Windows':
        os.remove(APP_ICON_PATH)
        os.remove(APP_ICON_OBJ)
