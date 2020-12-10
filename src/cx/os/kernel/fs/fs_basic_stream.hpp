//
//  fs_basic_stream.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 10.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_basic_stream_h
#define fs_basic_stream_h
#include <stdint.h>
#include <stddef.h>

namespace cx::os::kernel::fs
{
    using FsStreamDescriptor = uint64_t;
    
    class IFsBasicStream
    {
    public:
        virtual ~IFsBasicStream() = default;
        
        virtual bool IsOpen() const = 0;
        virtual void Close() = 0;
        
        virtual FsStreamDescriptor GetStreamDescriptor() const = 0;
    };
}

#endif /* fs_basic_stream_h */
