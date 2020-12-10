//
//  fs_node_type.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_node_type_h
#define fs_node_type_h

namespace cx::os::kernel::fs
{
    enum class FsNodeType
    {
        Directory,
        RegularFile,
        
        Socket,
        BlockDevice,
        CharacterDevice,
        
        SymLink,
    };
}

#endif /* fs_node_type_h */
