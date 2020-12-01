//
//  memory.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef memory_h
#define memory_h
#include <min_list.hpp>
#include <stddef.h>

namespace cx::os::kernel::memory
{
    struct MemoryRegion : MinListNode<MemoryRegion>
    {
        size_t size;
        bool free;
    };
    
    void  AddMemoryRegion(void* addr, size_t size);
    void  DumpMemoryRegions();
    
    void* AllocateMemory(size_t size);
    void  FreeMemory(void* addr);
}

extern "C" void* malloc(int);
extern "C" void free(void*);

#endif /* memory_h */
