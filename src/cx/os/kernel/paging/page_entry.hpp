//
//  page_entry.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef page_entry_h
#define page_entry_h
#include <stdint.hpp>

namespace cx::os::kernel::paging
{
    struct PageEntry
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
}

#endif /* page_entry_h */
