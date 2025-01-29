/*
 * Copyright (c) 2025 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#include "n8std/Unsafe.hpp"

#include <n8/ast/TerminativeSignal.hpp>
#include <unsafe_ops.hpp>

N8_FUNC(unsafe_volatileRead8) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0);
    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    double value = adrs.getNumber();
    auto addr = reinterpret_cast<volatile void*>(
        *reinterpret_cast<uintptr_t*>(&value)
    );

    return DynamicObject(
        static_cast<double>(volatile_read8(addr))
    );
}

N8_FUNC(unsafe_volatileRead16) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0);
    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    double value = adrs.getNumber();
    auto addr = reinterpret_cast<volatile void*>(
        *reinterpret_cast<uintptr_t*>(&value)
    );

    return DynamicObject(
        static_cast<double>(volatile_read16(addr))
    );
}

N8_FUNC(unsafe_volatileRead32) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0);
    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    double value = adrs.getNumber();
    auto addr = reinterpret_cast<volatile void*>(
        *reinterpret_cast<uintptr_t*>(&value)
    );

    return DynamicObject(
        static_cast<double>(volatile_read32(addr))
    );
}

N8_FUNC(unsafe_volatileWrite8) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile void*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    volatile_write8(
        addr,
        static_cast<uint8_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_volatileWrite16) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile void*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    volatile_write16(
        addr,
        static_cast<uint16_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_volatileWrite32) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile void*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    volatile_write32(
        addr,
        static_cast<uint32_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_registerSetBits) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Register parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile uint32_t*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    reg_set_bits(
        addr,
        static_cast<uint32_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_registerClearBits) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Register parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile uint32_t*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    reg_clear_bits(
        addr,
        static_cast<uint32_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_registerToggleBits) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Register parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile uint32_t*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    reg_toggle_bits(
        addr,
        static_cast<uint32_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_registerTestBits) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Register parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile uint32_t*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    reg_test_bits(
        addr,
        static_cast<uint32_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_registerReadField) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        value = args.at(1),
        mask = args.at(2);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Register parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    if(!mask.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Mask parameter should be of number type, got " +
                mask.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile uint32_t*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    reg_read_field(
        addr,
        static_cast<uint32_t>(value.getNumber()),
        static_cast<uint32_t>(mask.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_registerWriteField) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 4)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 4 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0),
        shift = args.at(1),
        value = args.at(2),
        mask = args.at(3);

    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Register parameter should be of number type, got " +
                adrs.objectType()
        );

    if(!shift.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Shift parameter should be of number type, got " +
                shift.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    if(!mask.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Mask parameter should be of number type, got " +
                mask.objectType()
        );

    double adrval = adrs.getNumber();
    auto addr = reinterpret_cast<volatile uint32_t*>(
        *reinterpret_cast<uintptr_t*>(&adrval)
    );

    reg_write_field(
        addr,
        static_cast<uint32_t>(mask.getNumber()),
        static_cast<uint32_t>(shift.getNumber()),
        static_cast<uint32_t>(value.getNumber())
    );

    return {};
}

N8_FUNC(unsafe_memoryBarrier) {
    N8_FUNC_REQUIRE_UNSAFE
    memory_barrier();

    return {};
}

N8_FUNC(unsafe_readBarrier) {
    N8_FUNC_REQUIRE_UNSAFE
    read_barrier();

    return {};
}

N8_FUNC(unsafe_writeBarrier) {
    N8_FUNC_REQUIRE_UNSAFE
    write_barrier();

    return {};
}

N8_FUNC(unsafe_memoryFenceAcquire) {
    N8_FUNC_REQUIRE_UNSAFE
    memory_fence_acquire();

    return {};
}

N8_FUNC(unsafe_memoryFenceRelease) {
    N8_FUNC_REQUIRE_UNSAFE
    memory_fence_release();

    return {};
}

N8_FUNC(unsafe_memoryFenceSequential) {
    N8_FUNC_REQUIRE_UNSAFE
    memory_fence_sequential();

    return {};
}

N8_FUNC(unsafe_enableInterrupts) {
    N8_FUNC_REQUIRE_UNSAFE
    enable_interrupts();

    return {};
}

N8_FUNC(unsafe_disableInterrupts) {
    N8_FUNC_REQUIRE_UNSAFE
    disable_interrupts();

    return {};
}

N8_FUNC(unsafe_setCpuAffinity) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject id = args.at(0);
    if(!id.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "CPU ID parameter should be of number type, got " +
                id.objectType()
        );

    set_cpu_affinity(
        static_cast<int>(id.getNumber())
    );
    return {};
}

N8_FUNC(unsafe_portRead8) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0);
    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    return DynamicObject(
        static_cast<double>(port_read8(
            static_cast<uint16_t>(adrs.getNumber())
        ))
    );
}

N8_FUNC(unsafe_portRead16) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0);
    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    return DynamicObject(
        static_cast<double>(port_read16(
            static_cast<uint16_t>(adrs.getNumber())
        ))
    );
}

N8_FUNC(unsafe_portRead32) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject adrs = args.at(0);
    if(!adrs.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Address parameter should be of number type, got " +
                adrs.objectType()
        );

    return DynamicObject(
        static_cast<double>(port_read32(
            static_cast<uint16_t>(adrs.getNumber())
        ))
    );
}

N8_FUNC(unsafe_portWrite8) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject port = args.at(0),
        value = args.at(1);

    if(!port.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Port parameter should be of number type, got " +
                port.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    port_write8(
        static_cast<uint16_t>(port.getNumber()),
        static_cast<uint8_t>(value.getNumber())
    );
    return {};
}

N8_FUNC(unsafe_portWrite16) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject port = args.at(0),
        value = args.at(1);

    if(!port.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Port parameter should be of number type, got " +
                port.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    port_write16(
        static_cast<uint16_t>(port.getNumber()),
        static_cast<uint16_t>(value.getNumber())
    );
    return {};
}

N8_FUNC(unsafe_portWrite32) {
    N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject port = args.at(0),
        value = args.at(1);

    if(!port.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Port parameter should be of number type, got " +
                port.objectType()
        );

    if(!value.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Value parameter should be of number type, got " +
                value.objectType()
        );

    port_write32(
        static_cast<uint16_t>(port.getNumber()),
        static_cast<uint32_t>(value.getNumber())
    );
    return {};
}

N8_FUNC(unsafe_inject) {
    //N8_FUNC_REQUIRE_UNSAFE
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject bytes = args.at(0),
        value = args.at(1);

    TerminativeThrowSignal notBytes(
        std::move(address),
        "Bytes parameter should be of number array type, got " +
            bytes.objectType()
    );

    if(!bytes.isArray())
        throw notBytes;

    std::vector<uint8_t> instBytes;
    for(const DynamicObject& obj : *bytes.getArray()) {
        if(!obj.isNumber())
            throw notBytes;

        instBytes.emplace_back(
            static_cast<uint8_t>(obj.getNumber())
        );
    }

    const std::string outType = value.toString();
    const size_t instSize = instBytes.size();
    uint8_t instructions[instSize];

    std::copy(
        instBytes.begin(),
        instBytes.end(),
        instructions
    );

    if(outType == "string")
        return DynamicObject(std::string(
            static_cast<const char*>(
                static_cast<const void*>(
                    execute_buffer<uint8_t*>(instructions, instSize)
                )
            ),
            instSize
        ));
    else if(outType == "number")
        return DynamicObject(static_cast<double>(
            execute_buffer<double>(instructions, instSize)
        ));
    else if(outType == "unit")
        execute_buffer<double>(instructions, instSize);

    return {};
}
