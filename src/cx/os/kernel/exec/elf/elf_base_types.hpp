//
//  elf_base_types.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 20.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef elf_base_types_h
#define elf_base_types_h
#include <stdint.h>

namespace cx::os::kernel::exec::elf
{
    enum class ElfCpuArchitecture : uint8_t
    {
        Arch32 = 1,
        Arch64 = 2
    };
    
    enum class ElfCpuByteOrder : uint8_t
    {
        LittleEndian = 1,
        BigEndian = 2
    };
    
    enum class ElfSystemAbi : uint8_t
    {
        SystemV = 0
    };
    
    enum class ElfFileType : uint16_t
    {
        Relocatable = 1,
        Executable = 2,
        Shared = 3,
        Core = 4
    };
    
    enum class ElfInstructionSet : uint16_t
    {
        Unspecified = 0x0,
        Sparc = 0x2,
        X86 = 0x3,
        Mips = 0x8,
        PowerPC = 0x14,
        ARM = 0x28,
        SuperH = 0x2A,
        Itanium64 = 0x32,
        Amd64 = 0x3E,
        Arm64 = 0xB7
    };
    
    enum ElfFileFlags : uint32_t
    {};
    
    using Elf32AddrType = uint32_t;
    using Elf64AddrType = uint64_t;
}

#endif /* elf_base_types_h */
