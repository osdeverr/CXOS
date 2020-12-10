//
//  fs.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 09.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_h
#define fs_h
#include <cx/os/kernel/fs/fs_directory.hpp>

namespace cx::os::kernel::fs
{
    FsDirectory& GetFilesystemRoot();
}

#endif /* fs_h */
