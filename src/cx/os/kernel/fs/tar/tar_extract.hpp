//
//  tar_extract.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 13.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef tar_extract_h
#define tar_extract_h
#include <cx/os/kernel/fs/tar/tar_file.hpp>
#include <cx/os/kernel/fs/fs_directory.hpp>

namespace cx::os::kernel::fs::tar
{
    bool ExtractTarballToDirectory(TarFile file, FsDirectory& directory);
}

#endif /* tar_extract_h */
