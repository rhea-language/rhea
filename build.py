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

import cpuinfo
import os
import platform
import shutil
import subprocess
import time

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

CORE_VERSION = '1.0.0'
OUT_DIR = os.path.join('dist', 'n8lang')

PLATFORM = platform.system()
ARCH = platform.architecture()[0]
OUTPUT_EXECUTABLE = os.path.join(OUT_DIR, 'bin', 'n8')
OUTPUT_LIBRARY = os.path.join(
    OUT_DIR,
    'modules', 'core@1.0.0',
    'lib', 'core'
)

cpp_files = []
cc_files = []

lib_headers = []
lib_source_files = []

def get_ext_instructions():
    log_task('Checking extended instruction availability...')
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
            print(
                "\033[1m[\033[0m \033[1;44m  TASK   \033[0m \033[1m]\033[0m \033[3m" +
                str(datetime.today()) + "\033[0m :: Checking extended instruction support for " +
                feature + "... ",
                end=''
            )

            if feature in cpu_info['flags']:
                supported_features.append('-m' + feature.replace('_', '.'))
                print('supported')
            else:
                print('not found')

    log_info("Done listing extended instruction support!")
    return supported_features

def include_local_lib(lib_name):
    global lib_headers
    global lib_source_files

    log_task("Listing included library folders...")
    lib_headers += ['-Ilib/' + lib_name + '/include']
    if os.path.exists('lib/' + lib_name + '/src'):
        lib_headers += ['-Ilib/' + lib_name + '/src']

    log_task("Listing included source files...")
    for root, dirs, files in os.walk('lib/' + lib_name + '/src'):
        for file in files:
            if file.endswith('.cpp') or file.endswith('.c'):
                lib_source_files.append(os.path.join(root, file))

    log_info("Done listing header file folders and source files!")

def build_proc():
    global OUTPUT_EXECUTABLE
    global OUTPUT_LIBRARY

    global cpp_files
    global cc_files

    global lib_headers
    global lib_source_files

    log_warning("Cleaning up already existing build folder...")
    if os.path.exists(OUT_DIR):
        shutil.rmtree(OUT_DIR)
    os.makedirs(OUT_DIR)
    log_info("Clean up done!")

    try:
        src = 'modules'

        log_task("Copying N8 standard library module files...")
        shutil.copytree(
            src,
            os.path.join(OUT_DIR, os.path.basename(src)),
            dirs_exist_ok=True
        )

        os.makedirs(os.path.join('dist', 'n8lang', 'bin'))
        os.makedirs(os.path.join(
            'dist', 'n8lang', 'modules',
            'core@1.0.0', 'lib'
        ))
        log_info(f"Successfully copied '{src}' to '{OUT_DIR}'")

    except Exception as e:
        log_error("Failed to copy N8 standard library modules.")
        exit(0)

    try:
        log_task("Pulling submodules...")
        subprocess.run(['git', 'submodule', '--quiet', 'update', '--init', '--recursive'])
        subprocess.run(['git', 'pull', '--quiet', '--recurse-submodules'])
        log_info("Done pulling up submodules!")

    except Exception as e:
        log_error(f"Failed to pull up submodules with error: {e}")
        exit(1)

    log_task("Listing included source file implementations...")
    for root, dirs, files in os.walk('src'):
        for file in files:
            if file.endswith('.cpp'):
                cpp_files.append(os.path.join(root, file))

    log_task("Listing included N8 standard library source files...")
    for root, dirs, files in os.walk('std'):
        for file in files:
            if file.endswith('.cc'):
                cc_files.append(os.path.join(root, file))

    log_task("Including local libraries/dependencies...")
    include_local_lib('chisei')
    include_local_lib('QuickDigest5')
    include_local_lib('quoneq')
    include_local_lib('MyShell')
    include_local_lib('unsafe_ops')
    log_info("Done including local libraries/dependencies!")

    try:
        ext_instructions = get_ext_instructions()
        if "TERMUX_VERSION" not in os.environ and PLATFORM == 'Linux':
            log_task("Installing GLFW3 package dependencies...")
            subprocess.run([
                'sudo', 'apt', 'install', '-y',
                'libglfw3-dev',
                'libgl1-mesa-dev',
                'libcurl4-gnutls-dev',
                'curl'
            ])
            log_info("GLFW3 package dependency installation done!")

        elif PLATFORM == 'Darwin':
            log_task("Installing GLFW3 and OpenSSL via brew...")
            subprocess.run(['brew', 'install', 'glfw', 'openssl'])
            subprocess.run(['brew', 'link', 'glfw'])
            log_info("Done installing GLFW3 and OpenSSL!")

        log_task('Building binaries...')
        if PLATFORM == 'Windows':
            now = time.time()

            config_res = 'dist\\n8-config.res'
            icon_config_res = 'dist\\n8-icon-config.res'

            win_libs = [
                '-static', '-static-libstdc++',
                '-lglfw3', '-lglew32', '-lopengl32', '-lgdi32',
                '-lwinmm', '-limm32', '-lole32', '-loleaut32',
                '-lversion', '-luuid', '-ldinput8', '-ldxguid',
                '-lsetupapi', '-lshell32', '-lssl', '-lcrypto',
                '-lcrypt32', '-lws2_32', '-luser32', '-lkernel32',
                '-lcurl'
            ]

            exe_build_args = [
                'g++', '-Iinclude', '-static', '-static-libstdc++',
                '-pedantic', '-Wall', '-Wdisabled-optimization',
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
                '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros', '-Wno-deprecated-declarations',
                '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-s', '-fopenmp'
            ] + ext_instructions + ['-mfpmath=sse',
                '-march=native', '-ffast-math'
            ] + lib_headers + lib_source_files + [
                config_res,
                icon_config_res
            ] + cpp_files + ['-o', OUTPUT_EXECUTABLE] + win_libs

            log_task("Generating Windows resource file configurations...")
            subprocess.run(['windres', 'configs\\n8-config.rc', '-O', 'coff', '-o', config_res])
            subprocess.run(['windres', 'configs\\n8-icon-config.rc', '-O', 'coff', '-o', icon_config_res])
            log_info("Windows resource file configurations successfully generated!")

            log_task("Building N8 core for Windows...")
            subprocess.run(exe_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

            log_warning("Cleaning up generated Windows resource file configurations...")
            os.remove(config_res)
            os.remove(icon_config_res)
            log_info("Clean up done!")

            now = time.time()
            lib_build_args = [
                'g++', '-Iinclude', '-Istd', '-shared',
                '-o', OUTPUT_LIBRARY + '.dll',
                '-Wno-deprecated-declarations'
            ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files + win_libs

            log_task("Building N8 standard library for Windows...")
            subprocess.run(lib_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

        elif "TERMUX_VERSION" in os.environ:
            now = time.time()

            ext_instructions.remove('-maes')
            exe_build_args= [
                'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
                '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
                '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror',
                '-Wfloat-equal', '-Wformat', '-Wformat=2', '-Wformat-nonliteral',
                '-Wformat-security', '-Wformat-y2k', '-Wimport', '-Winit-self',
                '-Winvalid-pch', '-Wno-header-guard', '-Wlong-long', '-Wno-deprecated-declarations',
                '-Wmissing-braces', '-Wmissing-field-initializers', '-Wmissing-format-attribute',
                '-Wmissing-include-dirs', '-Weffc++', '-Wpacked', '-Wparentheses',
                '-Wpointer-arith', '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
                '-Wshadow', '-Wsign-compare', '-Wstack-protector', '-Wstrict-aliasing',
                '-Wstrict-aliasing=2', '-Wswitch', '-Wswitch-default', '-Wswitch-enum',
                '-Wtrigraphs', '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
                '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
                '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros', '-O2',
                '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-ffast-math', '-s',
                '-std=c++23', '-fopenmp'] + ext_instructions + ['-march=native',
                '-ffast-math', '-D__TERMUX__'
            ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

            log_task("Building N8 core for Termux...")
            subprocess.run(exe_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

            now = time.time()
            lib_build_args = [
                'g++', '-Iinclude', '-Istd', '-fPIC', '-D__TERMUX__',
                '-shared', '-o', OUTPUT_LIBRARY + '.so',
                '-std=c++23', '-Wno-deprecated-declarations'
            ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files + [
                '-lcurl'
            ]

            log_task("Building N8 standard library for Termux...")
            subprocess.run(lib_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

        elif PLATFORM == 'Linux':
            now = time.time()

            exe_build_args= [
                'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
                '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
                '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror', '-Wno-deprecated-declarations',
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
                '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros', '-O2',
                '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-ffast-math', '-s',
                '-std=c++23', '-fopenmp'] + ext_instructions + ['-mfpmath=sse',
                '-march=native', '-ffast-math'
            ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

            log_task("Building N8 core for Linux...")
            subprocess.run(exe_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

            now = time.time()
            lib_build_args = [
                'g++', '-Iinclude', '-Istd', '-fPIC',
                '-shared', '-o', OUTPUT_LIBRARY + '.so',
                '-std=c++23', '-Wno-deprecated-declarations'
            ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files + [
                '-lglfw', '-lGL', '-lcurl'
            ]

            log_task("Building N8 standard library for Linux...")
            subprocess.run(lib_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

        elif PLATFORM == 'Darwin':
            now = time.time()

            lib_headers += [
                '-I/opt/homebrew/opt/openssl@3/include',
                '-I/opt/homebrew/Cellar/glfw/3.4/include/GLFW'
            ]

            exe_build_args = [
                '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude',
                '-pedantic', '-Wdisabled-optimization', '-pedantic-errors',
                '-Wextra', '-Wcast-align', '-Wcast-qual', '-Wchar-subscripts',
                '-Wcomment', '-Wconversion', '-Werror', '-Wfloat-equal',
                '-Wformat', '-Wformat=2', '-Wformat-nonliteral', '-Wformat-security',
                '-Wformat-y2k', '-Wimport', '-Winit-self', '-Winvalid-pch',
                '-Wlong-long', '-Wmissing-braces', '-Wmissing-field-initializers',
                '-Wmissing-format-attribute', '-Wmissing-include-dirs', '-Wpacked',
                '-Wparentheses', '-Wpointer-arith', '-Wredundant-decls', '-Wall',
                '-Wsequence-point', '-Wshadow', '-Wsign-compare', '-Wstack-protector',
                '-Wstrict-aliasing', '-Wstrict-aliasing=2', '-Wswitch',
                '-Wswitch-default', '-Wswitch-enum', '-Wtrigraphs', '-Wuninitialized',
                '-Wunknown-pragmas', '-Wunreachable-code', '-Wunused', '-Wno-deprecated-declarations',
                '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
                '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros',
                '-Wwrite-strings', '-Wno-return-type-c-linkage', '-pipe',
                '-std=c++23', '-fopenmp', '-march=native', '-ffast-math',
                '-flto=auto', '-Xpreprocessor', '-O2', '-Wno-header-guard',
                '-Wno-pessimizing-move'
            ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

            log_task("Building N8 core for MacOS...")
            subprocess.run(exe_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

            now = time.time()
            lib_build_args = [
                '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude',
                '-Istd', '-shared', '-o', OUTPUT_LIBRARY + '.dylib',
                '-Wno-deprecated-declarations', '-DGL_SILENCE_DEPRECATION',
                '-L/opt/homebrew/lib', '-L/opt/homebrew/opt/openssl@3/lib',
                '-std=c++23', '-Wno-deprecated-declarations'
            ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files + [
                '-lcrypto', '-lglfw', '-lcurl', '-framework', 'OpenGL'
            ]

            log_task("Building N8 standard library for MacOS...")
            subprocess.run(lib_build_args)
            end = time.time() - now
            log_info(f"Finished in {end:.6f} seconds")

    except Exception as e:
        log_error(f"Compilation failed with error: {e}")
        exit(1)

try:
    build_proc()
except Exception as e:
    log_error("Caught error: " + str(e))
except KeyboardInterrupt:
    log_error("Process interrupted. Exiting...")
