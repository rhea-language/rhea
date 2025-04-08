/*
 * Copyright (c) 2025 - Nathanne Isip
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

#include "rhea-std/Unsafe.hpp"

#include <rhea/ast/TerminativeSignal.hpp>
#include <unsafe_ops.hpp>

RHEA_FUNC(unsafe_volatileRead8) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_volatileRead16) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_volatileRead32) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_volatileWrite8) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_volatileWrite16) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_volatileWrite32) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_registerSetBits) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_registerClearBits) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_registerToggleBits) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_registerTestBits) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_registerReadField) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_registerWriteField) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_memoryBarrier) {
    RHEA_FUNC_REQUIRE_UNSAFE
    memory_barrier();

    return {};
}

RHEA_FUNC(unsafe_readBarrier) {
    RHEA_FUNC_REQUIRE_UNSAFE
    read_barrier();

    return {};
}

RHEA_FUNC(unsafe_writeBarrier) {
    RHEA_FUNC_REQUIRE_UNSAFE
    write_barrier();

    return {};
}

RHEA_FUNC(unsafe_memoryFenceAcquire) {
    RHEA_FUNC_REQUIRE_UNSAFE
    memory_fence_acquire();

    return {};
}

RHEA_FUNC(unsafe_memoryFenceRelease) {
    RHEA_FUNC_REQUIRE_UNSAFE
    memory_fence_release();

    return {};
}

RHEA_FUNC(unsafe_memoryFenceSequential) {
    RHEA_FUNC_REQUIRE_UNSAFE
    memory_fence_sequential();

    return {};
}

RHEA_FUNC(unsafe_enableInterrupts) {
    RHEA_FUNC_REQUIRE_UNSAFE
    enable_interrupts();

    return {};
}

RHEA_FUNC(unsafe_disableInterrupts) {
    RHEA_FUNC_REQUIRE_UNSAFE
    disable_interrupts();

    return {};
}

RHEA_FUNC(unsafe_setCpuAffinity) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_portRead8) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_portRead16) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_portRead32) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_portWrite8) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_portWrite16) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_portWrite32) {
    RHEA_FUNC_REQUIRE_UNSAFE
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

RHEA_FUNC(unsafe_inject) {
    RHEA_FUNC_REQUIRE_UNSAFE
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
    size_t instSize = instBytes.size();
    auto instructions = std::make_unique<uint8_t[]>(instSize);

    std::copy(
        instBytes.begin(),
        instBytes.end(),
        instructions.get()
    );

    if(outType == "string")
        return DynamicObject(std::string(
            static_cast<const char*>(
                static_cast<const void*>(
                    execute_buffer<uint8_t*>(instructions.get(), instSize)
                )
            ),
            instSize
        ));
    else if(outType == "number")
        return DynamicObject(static_cast<double>(
            execute_buffer<double>(instructions.get(), instSize)
        ));
    else if(outType == "unit")
        execute_buffer<double>(instructions.get(), instSize);

    return {};
}
