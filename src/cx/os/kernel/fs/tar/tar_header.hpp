//
//  tar_header.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 12.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef tar_header_h
#define tar_header_h
#include <stddef.h>

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
    
    template<size_t Length, size_t Base>
    class TarStringifiedNumber
    {
    public:
        size_t Get() const
        {
            unsigned int result = 0;
            unsigned int j;
            unsigned int count = 1;
            
            for (j = Length - 1; j > 0; j--, count *= Base)
                result += ((_str[j - 1] - '0') * count);
            
            return result;
        }
        
    private:
        char _str[Length];
    };
        
    template<size_t Length>
    using TarDecimal = TarStringifiedNumber<Length, 10>;
        
    template<size_t Length>
    using TarOctal = TarStringifiedNumber<Length, 8>;
    
    struct TarHeader
    {
        char file_name[100];
        char file_mode[8];
        
        TarDecimal<8> owner_uid;
        TarDecimal<8> owner_gid;
        
        TarOctal<12> file_size; // Octal
        TarOctal<12> file_time; // Octal
        
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
        
        void* GetFileContents()
        {
            return (this + 1);
        }
        
        const void* GetFileContents() const
        {
            return (this + 1);
        }
    };
}

#endif /* tar_header_h */
