//
//  fs_file.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_file_h
#define fs_file_h
#include <cx/os/kernel/fs/fs_node_impl.hpp>

namespace cx::os::kernel::fs
{
    class FsFile : public FsNodeImpl<FsFile>
    {
    public:
        FsFile(const FsNodeName& name, FsNode* parent = nullptr)
        : FsNodeImpl<FsFile>(name)
        {}
        
        static constexpr auto FsNodeImpl_NodeType = FsNodeType::File;
    };
}


#endif /* fs_file_h */
