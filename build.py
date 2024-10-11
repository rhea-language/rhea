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

PLATFORM = platform.system()
COMPILER = 'g++'
OUT_DIR = 'dist'
OUTPUT_EXECUTABLE = os.path.join(
    'dist',
    'zhivo-' +
        sys.platform + '-' +
        platform.machine().lower()
)

cpp_files = []
for root, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.cpp'):
            cpp_files.append(os.path.join(root, file))

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

rt_build_args = exe_build_args + cpp_files + [
    '-shared',
    '-o',
    os.path.join('dist', 'zhivocore.a'),
]

exe_build_args += cpp_files + ['-o', OUTPUT_EXECUTABLE]
cuda_build_args = ['nvcc']

if PLATFORM == 'Windows':
    cuda_build_args.append('-Xcompiler')
    cuda_build_args.append('/std:c++17')

cuda_build_args.append('-Iinclude')
cuda_build_args += cpp_files + ['-o', OUTPUT_EXECUTABLE + '-cuda']

try:
    os.makedirs(OUT_DIR, exist_ok=True)

    print("Executing:")
    print(' '.join(exe_build_args))
    subprocess.run(exe_build_args, check=True)

    print("Executing:")
    print(' '.join(rt_build_args))
    subprocess.run(rt_build_args, check=True)

    if PLATFORM != 'Darwin':
        print("Executing:")
        print(' '.join(cuda_build_args))
        subprocess.run(cuda_build_args, check=True)

except subprocess.CalledProcessError as e:
    print(f"Compilation failed with error: {e}")
