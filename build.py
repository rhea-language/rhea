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
import shutil
import subprocess
import sys

PLATFORM = platform.system()

OUT_DIR = 'dist'
if os.path.exists(OUT_DIR):
    shutil.rmtree(OUT_DIR)
os.makedirs('dist')

OUTPUT_EXECUTABLE = os.path.join(
    OUT_DIR,
    'zhivo-' +
        sys.platform + '-' +
        platform.machine().lower()
)
OUTPUT_LIBRARY = os.path.join(
    OUT_DIR,
    'zhivo-' +
        sys.platform + '-' +
        platform.machine().lower() +
        '-stdlib'
)
OUTPUT_CORE = OUTPUT_EXECUTABLE + '-core.a'

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

try:
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
            '-std=c++20', '-fopenmp', '-mmmx', '-msse', '-msse2', '-msse3', '-msse4',
            '-msse4.1', '-msse4.2', '-mavx', '-mavx2', '-mfma', '-mfpmath=sse',
            '-march=native', '-funroll-loops', '-ffast-math', '-static', '-static-libgcc',
            '-static-libstdc++'
        ] + cpp_files

        core_build_args = exe_build_args + [
            '-shared',
            '-o', OUTPUT_CORE
        ]
        exe_build_args += ['-o', OUTPUT_EXECUTABLE + '-openmp']
        lib_build_args = [
            'g++', '-Iinclude', '-Ilib', '-shared',
            '-o', OUTPUT_LIBRARY + '.dll', OUTPUT_CORE
        ] + cc_files

        cuda_exe_build_args = [
            'nvcc', '-x=cu', '-std=c++20',
            '--compiler-options=/openmp',
            '-Iinclude'
        ] + cpp_files + [
            '-lcudadevrt',
            '-lcudart_static',
            '-o'
        ] + [OUTPUT_EXECUTABLE + '-nvidia']

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(core_build_args))
        subprocess.run(core_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

        print("Executing:")
        print(' '.join(cuda_exe_build_args))
        subprocess.run(cuda_exe_build_args)

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
            '-std=c++20', '-fopenmp', '-mmmx', '-msse', '-msse2', '-msse3', '-msse4',
            '-msse4.1', '-msse4.2', '-mavx', '-mavx2', '-mfma', '-mfpmath=sse',
            '-march=native', '-funroll-loops', '-ffast-math'
        ] + cpp_files

        core_build_args = exe_build_args + [
            '-shared',
            '-o', OUTPUT_CORE
        ]
        exe_build_args += ['-o', OUTPUT_EXECUTABLE + '-openmp']
        lib_build_args = [
            'g++', '-Iinclude', '-Ilib', '-shared', '-fPIC',
            '-o', OUTPUT_LIBRARY + '.so', OUTPUT_CORE
        ] + cc_files

        cuda_exe_build_args = [
            'nvcc', '-x=cu', '-std=c++20',
            '--compiler-options=-fopenmp',
            '-Iinclude'
        ] + cpp_files + [
            '-lcudadevrt',
            '-lcudart_static',
            '-o'
        ] + [OUTPUT_EXECUTABLE + '-nvidia']

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(core_build_args))
        subprocess.run(core_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

        print("Executing:")
        print(' '.join(cuda_exe_build_args))
        subprocess.run(cuda_exe_build_args)
    
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
            '-Wreturn-type', '-Wsequence-point', '-Wshadow', '-Wsign-compare',
            '-Wstack-protector', '-Wstrict-aliasing', '-Wstrict-aliasing=2',
            '-Wswitch', '-Wswitch-default', '-Wswitch-enum', '-Wtrigraphs',
            '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
            '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros',
            '-Wwrite-strings', '-pipe', '-std=c++17', '-fopenmp', '-march=native',
            '-funroll-loops', '-ffast-math', '-flto=auto', '-Xpreprocessor',
            '-O3', '-Wno-header-guard', '-Wno-pessimizing-move'
        ] + cpp_files

        core_build_args = exe_build_args + ['-shared', '-o', OUTPUT_CORE]
        exe_build_args += ['-o', OUTPUT_EXECUTABLE + '-openmp']

        lib_build_args = [
            '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude',
            '-Ilib', '-shared', '-o', OUTPUT_LIBRARY + '.so',
            OUTPUT_CORE
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
