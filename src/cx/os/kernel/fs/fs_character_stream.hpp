//
//  fs_character_stream.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 10.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_character_stream_h
#define fs_character_stream_h
#include <stdint.h>
#include <stddef.h>

#include <cx/os/kernel/fs/fs_basic_stream.hpp>

namespace cx::os::kernel::fs
{
    // Can't use char here because EOF exists
    using FsCharacterByte = int16_t;
    constexpr FsCharacterByte kFsFileEOF = -1;
    
    enum FsSeekMode
    {
        FromStart,
        FromCurr,
        FromEnd
    };
    
    class IFsCharacterStream : public IFsBasicStream
    {
    public:
        virtual ~IFsCharacterStream() = default;
        
        virtual bool IsEOF() const = 0;
        
        virtual FsCharacterByte ReadByte() = 0;
        virtual void WriteByte(FsCharacterByte) = 0;
        
        size_t ReadBuffer(void* buffer, size_t size)
        {
            auto buf = reinterpret_cast<unsigned char*>(buffer);
            size_t i;
            
            for(i = 0; i < size; i++, buf++)
            {
                auto byte = ReadByte();
                if(IsEOF())
                    break;
                
                *buf = (unsigned char) byte;
            }
            
            return i;
        }
        
        size_t WriteBuffer(const void* buffer, size_t size)
        {
            auto buf = reinterpret_cast<const unsigned char*>(buffer);
            size_t i;
            
            for(i = 0; i < size; i++, buf++)
            {
                WriteByte(*buf);
                if(IsEOF())
                    break;
            }
            
            return i;
        }
    };
}

#endif /* fs_character_stream_h */
