//
//  fs_block_stream.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 10.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_block_stream_h
#define fs_block_stream_h
#include <cx/os/kernel/fs/fs_character_stream.hpp>

namespace cx::os::kernel::fs
{
    class IFsBlockStream : public IFsCharacterStream
    {
    public:
        virtual ~IFsBlockStream() = default;
        
        virtual size_t ReadBuffer(void* buffer, size_t size) = 0;
        virtual size_t WriteBuffer(const void* buffer, size_t size) = 0;
        
        virtual void Seek(FsSeekMode mode, size_t amount) = 0;
        virtual size_t Tell() const = 0;
    };
}

#endif /* fs_block_stream_h */
