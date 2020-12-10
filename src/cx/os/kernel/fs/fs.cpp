//
//  fs.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 09.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/fs/fs.hpp>

namespace cx::os::kernel::fs::detail
{
    // No name and no parents
    FsDirectory gTheFsRoot = {"", nullptr};
}

cx::os::kernel::fs::FsDirectory& cx::os::kernel::fs::GetFilesystemRoot()
{
    return detail::gTheFsRoot;
}
