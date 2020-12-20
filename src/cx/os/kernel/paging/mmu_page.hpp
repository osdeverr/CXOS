//
//  mmu_page.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef mmu_page_h
#define mmu_page_h
#include <stdint.h>

namespace cx::os::kernel::paging
{
    struct MmuPage
    {
        bool present : 1;
        bool read_write : 1;
        bool is_usermode : 1;
        bool write_through : 1;
        bool caching : 1;
        bool accessed : 1;
        bool dirty : 1;
        bool big_pages : 1;
        bool global : 1;
        
        int user_data : 3;
        
        uint32_t address : 20;
    };
    
    using MmuPageTable = MmuPage[1024];
    using MmuPageDirectory = MmuPage[1024];
}

#endif /* mmu_page_h */
