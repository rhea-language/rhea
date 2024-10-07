import os
import subprocess

OUT_DIR = 'dist'
OUTPUT_EXECUTABLE = os.path.join(OUT_DIR, 'zhivo-win-x86')

cpp_files = []
for root, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.cpp'):
            cpp_files.append(os.path.join(root, file))

if not cpp_files:
    print("No .cpp files found in the src directory.")
    exit(1)

gpp_command = [
    'g++',
    '-Iinclude',
    '-pipe',
    '-Ofast',
    '-g',
    '-s',
    '-std=c++17',
    '-fopenmp',
    '-msse',
    '-msse2',
    '-msse3',
    '-mfpmath=sse',
    '-flto=auto',
    '-march=native',
    '-funroll-loops',
    '-ffast-math',
    '-DNDEBUG'
] + cpp_files + ['-o', OUTPUT_EXECUTABLE]

print("Compiling with command:")
print(' '.join(gpp_command))

os.makedirs(OUT_DIR, exist_ok=True)

try:
    subprocess.run(gpp_command, check=True)
    print(f"Compilation successful! Executable created at: {OUTPUT_EXECUTABLE}")
except subprocess.CalledProcessError as e:
    print(f"Compilation failed with error: {e}")
