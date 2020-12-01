//
//  startup.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef startup_h
#define startup_h
#include <multiboot.h>

namespace cx::os::kernel
{
    void BeginKernelStartup(const multiboot_info_t& boot_info);
}

#endif /* startup_h */
