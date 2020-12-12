//
//  tar_header.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 12.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef tar_header_h
#define tar_header_h

namespace cx::os::kernel::fs::tar
{
    enum class TarFileType : char
    {
        Regular = '0',
        HardLink = '1',
        SymLink = '2',
        CharDevice = '3',
        BlockDevice = '4',
        Directory = '5',
        NamedPipe = '6'
    };
    
    struct TarHeader
    {
        char file_name[100];
        char file_mode[8];
        
        char owner_uid[8];
        char owner_gid[8];
        
        char file_size[12]; // Octal
        char file_time[12]; // Octal
        
        char checksum[8];
        
        TarFileType file_type;
        
        char linked_path[100];
        
        char ustar_magic[6];
        char ustar_version[2];
        
        char owner_user[32];
        char owner_group[32];
        
        char device_major[8];
        char device_minor[8];
        
        char file_prefix[155];
        
        char pad[12];
    };
}

#endif /* tar_header_h */
