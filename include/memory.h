//
//  memory.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef memory_h
#define memory_h
#include <stdint.h>
#include <tlist.h>

namespace Memory
{
    struct Region : public tList<Region>
    {
        uint32_t Size;
        bool Free;
    };
    
    void  AddRegion(void* start, uint32_t size);
    void* Allocate(uint32_t size);
    void  Free(void* ptr);
    
    void  DumpMemoryRegions();
}

void* malloc(uint32_t size);
void  free(void* ptr);

#endif /* memory_h */
