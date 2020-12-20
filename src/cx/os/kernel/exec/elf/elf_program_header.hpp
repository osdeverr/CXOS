//
//  elf_program_header.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 20.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef elf_program_header_h
#define elf_program_header_h
#include <stdint.h>
#include <cx/os/kernel/exec/elf/elf_base_types.hpp>

namespace cx::os::kernel::exec::elf
{
    enum class ElfSegmentType : uint32_t
    {
        Undefined = 0,
        StaticLoad = 1,
        DynamicLoad = 2,
        Interpret = 3,
    };
    
    enum ElfSegmentFlags : uint32_t
    {
        kElfSegmentFlags_Executable = 0x1,
        kElfSegmentFlags_Writable = 0x2,
        kElfSegmentFlags_Readable = 0x4,
    };
    
    // This is massive overengineering but then we don't have to repeat the middle path twice
    
    // The "middle" part of the program header structure - identical for both architectures
    template<class AddrT>
    struct ElfArchPartialProgramHeaderMiddle
    {
        AddrT offset;
        AddrT address;
        AddrT physical_address;
        AddrT file_size;
        AddrT mem_size;
    };
    
    // The 32-bit header start
    struct Elf32PartialProgramHeaderStart
    {
        ElfSegmentType type;
    };
    
    // The 32-bit header end
    struct Elf32PartialProgramHeaderEnd
    {
        ElfSegmentFlags flags;
        Elf32AddrType alignment;
    };
    
    // The 64-bit header start
    struct Elf64PartialProgramHeaderStart
    {
        ElfSegmentType type;
        ElfSegmentFlags flags;
    };
    
    // The 64-bit header end
    struct Elf64PartialProgramHeaderEnd
    {
        Elf64AddrType alignment;
    };
    
    // There we "glue" all 3 parts into one big struct
    struct Elf32ProgramHeader :
        Elf32PartialProgramHeaderStart,
        ElfArchPartialProgramHeaderMiddle<Elf32AddrType>,
        Elf32PartialProgramHeaderEnd
    {};
    
    struct Elf64ProgramHeader :
        Elf64PartialProgramHeaderStart,
        ElfArchPartialProgramHeaderMiddle<Elf64AddrType>,
        Elf64PartialProgramHeaderEnd
    {};
}

#endif /* elf_program_header_h */
