//
//  elf_file_header.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 20.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef elf_file_header_h
#define elf_file_header_h
#include <stdint.h>
#include <cx/os/kernel/exec/elf/elf_base_types.hpp>

namespace cx::os::kernel::exec::elf
{    
    struct ElfBaseFileHeader
    {
        char magic[4]; // '\x7fELF'
        
        ElfCpuArchitecture architecture;
        ElfCpuByteOrder byte_order;
        uint8_t header_version;
        ElfSystemAbi abi_type;
        
        uint64_t padding;
    };
    
    template<class AddrT>
    struct ElfArchFileHeader
    {
        ElfFileType file_type;
        ElfInstructionSet instruction_set;
        uint32_t elf_version;
        AddrT entry_point;
        AddrT ph_offset;
        AddrT sh_offset;
        ElfFileFlags flags;
        uint16_t header_size;
        
        uint16_t ph_entry_size;
        uint16_t ph_entry_num;
        uint16_t sh_entry_size;
        uint16_t sh_entry_num;
        
        uint16_t sh_names_index;
    };
    
    using Elf32FileHeader = ElfArchFileHeader<uint32_t>;
    using Elf64FileHeader = ElfArchFileHeader<uint64_t>;
}

#endif /* elf_file_header_h */
