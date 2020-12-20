//
//  paging.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 20.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/paging/mmu_page.hpp>

namespace cx::os::kernel::paging::detail
{
    MmuPageTable gThePageTables[1024];
    MmuPageDirectory gThePageDirectory;
}
