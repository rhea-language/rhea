/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Rhea.
 * 
 * Rhea is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Rhea is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Rhea. If not, see <https://www.gnu.org/licenses/>.
 */

#include "rhea-std/Sys.hpp"

#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/util/RandomUtil.hpp>

#include <cstdlib>
#include <fstream>
#include <myshell.hpp>
#include <unordered_map>

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <windows.h>
#endif

RHEA_FUNC(sys_quickShell) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    return DynamicObject(
        static_cast<double>(system(
            value.toString().c_str()
        ))
    );
}

RHEA_FUNC(sys_shellConnect) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = RheaUtil::uniqueKey();

    shellMap[uuid] = std::make_shared<MyShell>(
        value.toString()
    );
    return DynamicObject(uuid);
}

RHEA_FUNC(sys_shellWrite) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0),
        content = args.at(1);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    shellMap[uuid]->writeToShell(content.toString());
    return DynamicObject(uuid);
}

RHEA_FUNC(sys_shellReadOutput) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        shellMap[uuid]->readShellOutputStream()
    );
}

RHEA_FUNC(sys_shellReadError) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        shellMap[uuid]->readShellErrorStream()
    );
}

RHEA_FUNC(sys_shellForceExit) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    shellMap[uuid]->forceExit();
    return DynamicObject(uuid);
}

RHEA_FUNC(sys_shellHasExited) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(shellMap[uuid]->hasExited());
}

RHEA_FUNC(sys_shellExitCode) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        static_cast<double>(
            shellMap[uuid]->exitCode()
        )
    );
}

RHEA_FUNC(sys_shellProcessId) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    return DynamicObject(
        static_cast<double>(
            shellMap[uuid]->processId()
        )
    );
}

RHEA_FUNC(sys_shellClose) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    std::string uuid = value.toString();

    if(shellMap.count(uuid) != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Shell handler not found"
        );

    shellMap.erase(uuid);
    return DynamicObject(uuid);
}

RHEA_FUNC(sys_arch) {
    std::string architecture;

    #if defined(__x86_64__) || defined(_M_X64)
        architecture = "x86_64";
    #elif defined(__i386__) || defined(_M_IX86)
        architecture = "x86_32";
    #elif defined(__aarch64__) || defined(_M_ARM64)
        architecture = "arm64";
    #elif defined(__arm__) || defined(_M_ARM)
        #if defined(__ARM_ARCH_7__)
            architecture = "armv7";
        #elif defined(__ARM_ARCH_7A__)
            architecture = "armv7a";
        #elif defined(__ARM_ARCH_7R__)
            architecture = "armv7r";
        #elif defined(__ARM_ARCH_7M__)
            architecture = "armv7m";
        #elif defined(__ARM_ARCH_6__)
            architecture = "armv6";
        #elif defined(__ARM_ARCH_5TE__)
            architecture = "armv5te";
        #else
            architecture = "arm32";
        #endif
    #elif defined(__powerpc64__) || defined(__ppc64__)
        #if defined(__LITTLE_ENDIAN__)
            architecture = "powerpc64le";
        #else
            architecture = "powerpc64";
        #endif
    #elif defined(__powerpc__) || defined(__ppc__)
        architecture = "powerpc";
    #elif defined(__sparc__)
        #if defined(__sparc_v9__)
            architecture = "sparc64";
        #else
            architecture = "sparc";
        #endif
    #elif defined(__mips__)
        #if defined(__mips64)
            #if defined(__MIPSEL__)
                architecture = "mips64el";
            #else
                architecture = "mips64";
            #endif
        #else
            #if defined(__MIPSEL__)
                architecture = "mipsel";
            #else
                architecture = "mips";
            #endif
        #endif
    #elif defined(__riscv)
        #if defined(__riscv_xlen) && __riscv_xlen == 64
            architecture = "riscv64";
        #elif defined(__riscv_xlen) && __riscv_xlen == 32
            architecture = "riscv32";
        #else
            architecture = "riscv";
        #endif
    #elif defined(__s390__)
        architecture = "s390";
    #elif defined(__alpha__)
        architecture = "alpha";
    #elif defined(__m68k__)
        architecture = "m68k";
    #elif defined(__ia64__) || defined(_IA64)
        architecture = "ia64";
    #elif defined(__loongarch64)
        architecture = "loongarch64";
    #elif defined(__loongarch32)
        architecture = "loongarch32";
    #elif defined(__e2k__)
        architecture = "elbrus";
    #elif defined(__pa_risc__) || defined(__hppa__)
        #if defined(__LP64__)
            architecture = "pa_risc64";
        #else
            architecture = "pa_risc";
        #endif
    #elif defined(__sh__)
        #if defined(__SH5__)
            architecture = "superh5";
        #elif defined(__SH4__)
            architecture = "superh4";
        #else
            architecture = "superh";
        #endif
    #elif defined(_TMS320C6X)
        architecture = "tms320c6x";
    #elif defined(__nds32__) || defined(__NDS32__)
        #if defined(__NDS32_VER__)
            architecture = "andes_nds32";
        #else
            architecture = "andes";
        #endif
    #elif defined(__arc__)
        #if defined(__ARC64_ARCH__)
            architecture = "arc64";
        #else
            architecture = "arc";
        #endif
    #elif defined(__csky__)
        architecture = "csky";
    #else
        architecture = "unknown";
    #endif

    return DynamicObject(architecture);
}

RHEA_FUNC(sys_platform) {
    std::string name = "unknown",
        variant = "unknown",
        version = "unknown",
        kernel = "unknown";

    #if defined(_WIN32) || defined(_WIN64)
        name = "Windows";

        #if defined(WINAPI_FAMILY)
            #if defined(WINAPI_FAMILY_PC_APP)
                variant = "Universal Windows Platform";
            #elif defined(WINAPI_FAMILY_PHONE_APP)
                variant = "Windows Phone";
            #elif defined(WINAPI_FAMILY_SERVER)
                variant = "Windows Server";
            #else
                variant = "Windows Desktop";
            #endif
        #endif

        #ifdef _WIN64
            kernel = "ntoskrnl64";
        #else
            kernel = "ntoskrnl";
        #endif

    #elif defined(__APPLE__) && defined(__MACH__)
        #include <TargetConditionals.h>
        kernel = "XNU";

        #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
            name = "iOS";
            variant = "Simulator";
        #elif TARGET_OS_IPHONE
            name = "iOS";
            variant = "Device";
        #elif TARGET_OS_MAC
            name = "macOS";
            variant = "Desktop";
        #elif TARGET_OS_TV
            name = "tvOS";
        #elif TARGET_OS_WATCH
            name = "watchOS";
        #endif

    #elif defined(__linux__)
        name = "Linux";

        #if defined(__ANDROID__)
            variant = "Android";
        #elif defined(__CHROMEOS__)
            variant = "ChromeOS";
        #else
            #if defined(__DEBIAN__)
                variant = "Debian";
            #elif defined(__UBUNTU__)
                variant = "Ubuntu";
            #elif defined(__KUBUNTU__)
                variant = "Kubuntu";
            #elif defined(__XUBUNTU__)
                variant = "Xubuntu";
            #elif defined(__LUBUNTU__)
                variant = "Lubuntu";
            #elif defined(__LINUX_MINT__)
                variant = "Linux Mint";
            #elif defined(__ELEMENTARY__)
                variant = "Elementary OS";
            #elif defined(__POP_OS__)
                variant = "Pop!_OS";
            #elif defined(__ZORIN__)
                variant = "Zorin OS";
            #elif defined(__MX_LINUX__)
                variant = "MX Linux";
            #elif defined(__DEEPIN__)
                variant = "Deepin";
            #elif defined(__FEDORA__)
                variant = "Fedora";
            #elif defined(__REDHAT__)
                variant = "Red Hat Enterprise Linux";
            #elif defined(__CENTOS__)
                variant = "CentOS";
            #elif defined(__ROCKY__)
                variant = "Rocky Linux";
            #elif defined(__ALMA__)
                variant = "AlmaLinux";
            #elif defined(__ORACLE_LINUX__)
                variant = "Oracle Linux";
            #elif defined(__AMAZON_LINUX__)
                variant = "Amazon Linux";
            #elif defined(__SCIENTIFIC_LINUX__)
                variant = "Scientific Linux";
            #elif defined(__OPENSUSE__)
                variant = "openSUSE";
            #elif defined(__SUSE_ENTERPRISE__)
                variant = "SUSE Linux Enterprise";
            #elif defined(__ARCH__)
                variant = "Arch Linux";
            #elif defined(__MANJARO__)
                variant = "Manjaro";
            #elif defined(__ENDEAVOUR__)
                variant = "EndeavourOS";
            #elif defined(__GARUDA__)
                variant = "Garuda Linux";
            #elif defined(__ARTIX__)
                variant = "Artix Linux";
            #elif defined(__GENTOO__)
                variant = "Gentoo";
            #elif defined(__SABAYON__)
                variant = "Sabayon";
            #elif defined(__CALCULATE__)
                variant = "Calculate Linux";
            #elif defined(__SLACKWARE__)
                variant = "Slackware";
            #elif defined(__VOID__)
                variant = "Void Linux";
            #elif defined(__SOLUS__)
                variant = "Solus";
            #elif defined(__CLEAR_LINUX__)
                variant = "Clear Linux";
            #elif defined(__PUPPY__)
                variant = "Puppy Linux";
            #elif defined(__TINYCORE__)
                variant = "Tiny Core Linux";
            #elif defined(__ALPINE__)
                variant = "Alpine Linux";
            #elif defined(__ANTIX__)
                variant = "antiX";
            #elif defined(__KALI__)
                variant = "Kali Linux";
            #elif defined(__PARROT__)
                variant = "Parrot OS";
            #elif defined(__BLACKARCH__)
                variant = "BlackArch Linux";
            #elif defined(__PENTOO__)
                variant = "Pentoo";
            #elif defined(__QUBES__)
                variant = "Qubes OS";
            #elif defined(__TAILS__)
                variant = "Tails";
            #elif defined(__WHONIX__)
                variant = "Whonix";
            #elif defined(__PROXMOX__)
                variant = "Proxmox VE";
            #elif defined(__OPENMEDIAVAULT__)
                variant = "OpenMediaVault";
            #elif defined(__OPENWRT__)
                variant = "OpenWrt";
            #elif defined(__NIXOS__)
                variant = "NixOS";
            #elif defined(__GUIX__)
                variant = "Guix System";
            #elif defined(__BEDROCK__)
                variant = "Bedrock Linux";
            #elif defined(__GENTOO_PREFIX__)
                variant = "Gentoo Prefix";
            #elif defined(__MAGEIA__)
                variant = "Mageia";
            #elif defined(__PCLINUXOS__)
                variant = "PCLinuxOS";
            #elif defined(__ROSA__)
                variant = "ROSA Linux";
            #elif defined(__MANDRIVA__)
                variant = "Mandriva Linux";
            #elif defined(__OPENMANDRIVA__)
                variant = "OpenMandriva";
            #elif defined(__PORTEUS__)
                variant = "Porteus";
            #elif defined(__SLAX__)
                variant = "Slax";
            #elif defined(__KNOPPIX__)
                variant = "Knoppix";
            #elif defined(__PEPPERMINT__)
                variant = "Peppermint OS";
            #elif defined(__BODHI__)
                variant = "Bodhi Linux";
            #elif defined(__SPARKY__)
                variant = "SparkyLinux";
            #elif defined(__Q4OS__)
                variant = "Q4OS";
            #elif defined(__KAOS__)
                variant = "KaOS";
            #elif defined(__CHAKRA__)
                variant = "Chakra Linux";
            #elif defined(__CLEAROS__)
                variant = "ClearOS";
            #elif defined(__ENDLESS__)
                variant = "Endless OS";
            #elif defined(__CHROMEOS__)
                variant = "Chrome OS";
            #else
                std::ifstream osReleaseFile("/etc/os-release");
                if(osReleaseFile.is_open()) {
                    std::string line;
                    while(std::getline(osReleaseFile, line))
                        if(line.find("PRETTY_NAME=") == 0) {
                            std::string distroName = line.substr(13);

                            if(!distroName.empty() && distroName.back() == '"')
                                distroName.pop_back();

                            variant = distroName;
                        }
                }
            #endif
        #endif

    #elif defined(__FreeBSD__)
        name = "FreeBSD";

        #if defined(__FreeBSD_version)
            version = __FreeBSD_version;
        #endif

    #elif defined(__DragonFly__)
        name = "DragonFlyBSD";
        kernel = "DragonFly";

    #elif defined(__NetBSD__)
        name = "NetBSD";

    #elif defined(__OpenBSD__)
        name = "OpenBSD";

    #elif defined(__sun) && defined(__SVR4)
        name = "Solaris";

        #if defined(__illumos__)
            variant = "illumos";
        #else
            variant = "Oracle Solaris";
            kernel = "SunOS";
        #endif

    #elif defined(_AIX)
        name = "AIX";

    #elif defined(__hpux)
        name = "HP-UX";

    #elif defined(__HAIKU__)
        name = "Haiku";

    #elif defined(__QNX__)
        name = "QNX";
        #if defined(__QNXNTO__)
            variant = "Neutrino";
        #endif

    #elif defined(__CYGWIN__)
        name = "Cygwin";
        variant = "POSIX (Windows/Cygwin)";

    #elif defined(__MSYS__)
        name = "MSYS";
        variant = "POSIX (Windows/msys)";

    #elif defined(__gnu_hurd__)
        name = "GNU/Hurd";
        kernel = "Hurd";

    #elif defined(__INTEGRITY)
        name = "INTEGRITY";

    #elif defined(__Fuchsia__)
        name = "Fuchsia";
        kernel = "Zircon";

    #elif defined(__VXWORKS__)
        name = "VxWorks";

    #elif defined(__NUTTX__)
        name = "NuttX";

    #elif defined(__ZEPHYR__)
        name = "Zephyr";

    #elif defined(__MBED__)
        name = "Mbed OS";

    #elif defined(__MINIX)
        name = "MINIX";

    #elif defined(__RISCOS)
        name = "RISC OS";

    #elif defined(__plan9__)
        name = "Plan 9";

    #elif defined(__CloudABI__)
        name = "CloudABI";

    #elif defined(__serenity__)
        name = "SerenityOS";

    #elif defined(__redox__)
        name = "Redox";

    #elif defined(__hermit__)
        name = "HermitCore";
    #endif

    #ifdef __linux__
        kernel = "linux";
    #elif defined(__FreeBSD_kernel__)
        kernel = "kernel";
    #elif defined(__NetBSD_kernel__)
        kernel = "netbsd";
    #elif defined(__OpenBSD_kernel__)
        kernel = "bsd";
    #endif

    std::vector<DynamicObject> info;
    info.emplace_back(DynamicObject(name));
    info.emplace_back(DynamicObject(variant));
    info.emplace_back(DynamicObject(version));
    info.emplace_back(DynamicObject(kernel));

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            info
        )
    );
}

RHEA_FUNC(sys_wordSize) {
    return DynamicObject(
        static_cast<double>(sizeof(void*) * 8)
    );
}

RHEA_FUNC(sys_endianess) {
    unsigned int x = 1;
    char *c = (char*) &x;

    return DynamicObject(
        (*c == 1) ? "little-endian" : "big-endian"
    );
}

RHEA_FUNC(sys_cpuFeatures) {
    std::vector<DynamicObject> features;

    #ifdef __ABM__
        features.emplace_back(DynamicObject(std::string("abm")));
    #endif

    #ifdef __ADX__
        features.emplace_back(DynamicObject(std::string("adx")));
    #endif

    #ifdef __AES__
        features.emplace_back(DynamicObject(std::string("aes")));
    #endif

    #ifdef __AVX__
        features.emplace_back(DynamicObject(std::string("avx")));
    #endif

    #ifdef __AVX2__
        features.emplace_back(DynamicObject(std::string("avx2")));
    #endif

    #ifdef __BMI__
        features.emplace_back(DynamicObject(std::string("bmi")));
    #endif

    #ifdef __CLFLUSHOPT__
        features.emplace_back(DynamicObject(std::string("clflushopt")));
    #endif

    #ifdef __CX16__
        features.emplace_back(DynamicObject(std::string("cx16")));
    #endif

    #ifdef __F16C__
        features.emplace_back(DynamicObject(std::string("f16c")));
    #endif

    #ifdef __FMA__
        features.emplace_back(DynamicObject(std::string("fma")));
    #endif

    #ifdef __FSGSBASE__
        features.emplace_back(DynamicObject(std::string("fsgsbase")));
    #endif

    #ifdef __FXSR__
        features.emplace_back(DynamicObject(std::string("fxsr")));
    #endif

    #ifdef __MMX__
        features.emplace_back(DynamicObject(std::string("mmx")));
    #endif

    #ifdef __MOVBE__
        features.emplace_back(DynamicObject(std::string("movbe")));
    #endif

    #ifdef __RDRND__
        features.emplace_back(DynamicObject(std::string("rdrnd")));
    #endif

    #ifdef __RDSEED__
        features.emplace_back(DynamicObject(std::string("rdseed")));
    #endif

    #ifdef __RTM__
        features.emplace_back(DynamicObject(std::string("rtm")));
    #endif

    #ifdef __SGX__
        features.emplace_back(DynamicObject(std::string("sgx")));
    #endif

    #ifdef __SSE__
        features.emplace_back(DynamicObject(std::string("sse")));
    #endif

    #ifdef __SSE2__
        features.emplace_back(DynamicObject(std::string("sse2")));
    #endif

    #ifdef __SSE3__
        features.emplace_back(DynamicObject(std::string("sse3")));
    #endif

    #ifdef __SSE4_1__
        features.emplace_back(DynamicObject(std::string("sse4_1")));
    #endif

    #ifdef __SSE4_2__
        features.emplace_back(DynamicObject(std::string("sse4_2")));
    #endif

    #ifdef __SSE4__
        features.emplace_back(DynamicObject(std::string("sse4")));
    #endif

    #ifdef __VAES__
        features.emplace_back(DynamicObject(std::string("vaes")));
    #endif

    #ifdef __VPCLMULQDQ__
        features.emplace_back(DynamicObject(std::string("vpclmulqdq")));
    #endif

    #ifdef __XSAVE__
        features.emplace_back(DynamicObject(std::string("xsave")));
    #endif

    #ifdef __XSAVEC__
        features.emplace_back(DynamicObject(std::string("xsavec")));
    #endif

    #ifdef __XSAVEOPT__
        features.emplace_back(DynamicObject(std::string("xsaveopt")));
    #endif

    #if defined(__x86_64__) || defined(__i386__)
    unsigned int eax, ebx, ecx, edx;
    __asm__ __volatile__ (
        "cpuid"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (1)
    );

    if(ecx & (1 << 25))
        features.emplace_back(DynamicObject(std::string("aes")));
    if(ecx & (1 << 1))
        features.emplace_back(DynamicObject(std::string("pclmulqdq")));
    if(ecx & (1 << 23))
        features.emplace_back(DynamicObject(std::string("popcnt")));

    __asm__ __volatile__ (
        "cpuid"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (7), "c" (0)
    );

    if(ebx & (1 << 5))
        features.emplace_back(DynamicObject(std::string("avx2")));
    if(ebx & (1 << 19))
        features.emplace_back(DynamicObject(std::string("adx")));
    if(ebx & (1 << 18))
        features.emplace_back(DynamicObject(std::string("rdseed")));
    #endif

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            features
        )
    );
}

RHEA_FUNC(sys_sleep) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject value = args.at(0);
    #if defined(_WIN32) || defined(_WIN64)
    Sleep(
        static_cast<DWORD>(value.getNumber())
    );
    #else
    std::this_thread::sleep_for(
        std::chrono::milliseconds(
            static_cast<int64_t>(value.getNumber())
        )
    );
    #endif

    return {};
}
