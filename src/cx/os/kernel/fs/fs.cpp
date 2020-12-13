//
//  fs.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 09.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/fs/fs.hpp>
#include <cx/stl/list.hpp>

namespace cx::os::kernel::fs::detail
{
    // No name and no parents
    FsDirectory gTheFsRoot = {"", nullptr};
    
    struct OpenedFileStream
    {
        FsStreamDescriptor fd = 0;
        
        IFsCharacterStream* char_stream = nullptr;
        IFsBlockStream* block_stream = nullptr;
    };
    
    std::list<OpenedFileStream> gOpenedFiles;
}

cx::os::kernel::fs::FsDirectory& cx::os::kernel::fs::GetFilesystemRoot()
{
    return detail::gTheFsRoot;
}

cx::std::shared_ptr<cx::os::kernel::fs::FsNode> FindFilesystemNode(const char* path)
{
    
    return nullptr;
}
