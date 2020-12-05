//
//  memory.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <cx/os/kernel/memory/memory.hpp>
#include <cx/os/kernel/kprintf.hpp>

namespace cx::os::kernel::memory::detail
{
    MinList<MemoryRegion> gTheMemoryRegions;
}

void cx::os::kernel::memory::AddMemoryRegion(void* addr, size_t size)
{
    auto region = reinterpret_cast<MemoryRegion*>(addr);
    region->size = size - sizeof(MemoryRegion);
    region->free = true;
    
    detail::gTheMemoryRegions.InsertNode(region);
}

void cx::os::kernel::memory::DumpMemoryRegions()
{
    printf("\n");
    for(auto& region : detail::gTheMemoryRegions)
        kprintf("DumpMemoryRegions: Region @ 0x%08X (size=0x%08X bytes) -> free=%d\n", &region, region.size, (int) region.free);
    printf("\n");
}

void* cx::os::kernel::memory::AllocateMemory(size_t size)
{    
    // Allocating sizes <= sizeof(MemoryRegion) destroys structures if we don't do this
    size += (size % sizeof(MemoryRegion));
    
    for(auto& region : detail::gTheMemoryRegions)
    {
        if(region.free && region.size >= size)
        {
            // kprintf("AllocateMemory (n=%d): Region @ 0x%08X (n=0x%08X) -> free=%d\n", size, &region, region.size, (int) region.free);
            region.free = false;
            
            auto data = (&region + 1);
            auto diff = region.size - size;
            region.size = size;
            if(diff)
                AddMemoryRegion((char*) data + size, diff);
            
            // DumpMemoryRegions();
            return data;
        }
    }
    
    return nullptr;
}

void cx::os::kernel::memory::FreeMemory(void* addr)
{
    auto region = reinterpret_cast<MemoryRegion*>((char*) addr - sizeof(MemoryRegion));
    region->free = true;
    // kprintf("FreeMemory: Region @ 0x%08X (size=0x%08X bytes) -> free=%d\n", region, region->size, (int) region->free);
}
 
extern "C" void* malloc(int size)
{
    return cx::os::kernel::memory::AllocateMemory(size);
}
extern "C" void free(void* ptr)
{
    cx::os::kernel::memory::FreeMemory(ptr);
}
