//
//  tar_file.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 13.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef tar_file_h
#define tar_file_h
#include <cx/os/kernel/fs/tar/tar_header.hpp>
#include <string.h>

namespace cx::os::kernel::fs::tar
{
    class TarFile
    {
    public:
        TarFile(const void* data)
        : _data((const char*) data)
        {}
        
        const TarHeader* GetNext()
        {
            auto header = reinterpret_cast<const TarHeader*>(_data);
            if(!strcmp(header->ustar_magic, "ustar"))
            {
                auto size = header->file_size.Get();
                
                _data += sizeof(TarHeader);
                _data += ((size / 512)) * 512;
                
                if (size % 512)
                    _data += 512;
                
                return header;
            }
            else
            {
                return nullptr;
            }
        }
        
    private:
        const char* _data;
    };
}

#endif /* tar_file_h */
