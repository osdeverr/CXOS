//
//  memory.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <memory.h>
#include <assert.h>
#include <itoa.h>
#include <console.h>
#include <printf.h>

namespace Memory
{
    tList<Region> gRegionList;
}
void Memory::AddRegion(void* start, uint32_t size)
{
    Region* pReg = (Region*) start;
    pReg->Size = size - sizeof(Region);
    pReg->Free = true;
    
    gRegionList.Insert(pReg);
}
void* Memory::Allocate(uint32_t size)
{
    assert(Memory::gRegionList.Valid());
    
    Region* pReg = gRegionList.Prev->Next;
    while(pReg != gRegionList.Prev)
    {
        Region& region = *(pReg);
        pReg = pReg->Next;
        if(region.Size < size || !region.Free)
            continue;
        
        auto ptr = &region + 1;
        printf("*** MEM: Allocating memory region 0x%08X (rs=0x%08X as=0x%08X)\n", ptr, region.Size, size);
        AddRegion((char*) &region + size + sizeof(Region), region.Size - size);
        region.Size = size;
        region.Free = false;
        return ptr; // sizeof region
    };
    
    panic("Kernel out of physical memory");
    return nullptr;
}
void Memory::Free(void* ptr)
{
    Region& region = *((Region*) ptr - 1);
    
    printf("*** MEM: Freeing memory region 0x%08X (rs=0x%08X)\n", ptr, region.Size);
    
    region.Free = true;
}
void Memory::DumpMemoryRegions()
{
    printf("\n");
    
    int i = 1;
    Region* pReg = gRegionList.Prev->Next;
    while(pReg != gRegionList.Prev)
    {
        Region& region = *(pReg);
        pReg = pReg->Next;
        
        auto ptr = &region + 1;
        printf("*** MEM: Region #%d => {addr=0x%08X rs=0x%08X free=%d}\n", i++, ptr, region.Size, (int) region.Free);
    };
    
    printf("\n");
}

void* malloc(uint32_t size) { return Memory::Allocate(size); }
void  free(void* ptr) { Memory::Free(ptr); }
