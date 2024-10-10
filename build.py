import os
import platform
import subprocess
import sys

OUT_DIR = 'dist'
OUTPUT_EXECUTABLE = os.path.join(
    'dist',
    'zhivo-' +
        sys.platform + '-' +
        platform.machine().lower()
)
COMPILER = 'g++'

cpp_files = []
for root, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.cpp'):
            cpp_files.append(os.path.join(root, file))

if not cpp_files:
    print("No .cpp files found in the src directory.")
    exit(1)

if platform.system() == 'Darwin':
    COMPILER = '/opt/homebrew/opt/llvm/bin/clang++'

gpp_command = [
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
    '-pipe', '-Ofast', '-g', '-s', '-std=c++17', '-fopenmp',
    '-msse', '-msse2', '-msse3', '-mfpmath=sse',
    '-march=native', '-funroll-loops', '-ffast-math',
    '-DNDEBUG'
]

if platform.system() != 'Windows':
    gpp_command.append('-flto=auto')

if platform.system() == 'Darwin':
    gpp_command.append('-Xpreprocessor')
    gpp_command.append('-O3')
    gpp_command.remove('-Ofast')
    gpp_command.remove('-msse')
    gpp_command.remove('-msse2')
    gpp_command.remove('-msse3')

gpp_command += cpp_files + ['-o', OUTPUT_EXECUTABLE]
nvcc_command = [
    'nvcc', '-Iinclude'
] + cpp_files + ['-o', OUTPUT_EXECUTABLE + '-cuda']

os.makedirs(OUT_DIR, exist_ok=True)

try:
    print("Compiling with command:")
    print(' '.join(gpp_command))

    subprocess.run(gpp_command, check=True)
    print(f"Compilation successful! Executable created at: {OUTPUT_EXECUTABLE}")

    print("Compiling with command:")
    print(' '.join(nvcc_command))

    subprocess.run(nvcc_command, check=True)
    print(f"Compilation successful! Executable created at: {OUTPUT_EXECUTABLE + '-cuda'}")

except subprocess.CalledProcessError as e:
    print(f"Compilation failed with error: {e}")
