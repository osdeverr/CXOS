//
//  gdt.c
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/21/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//
#include <gdt.h>

extern "C" {
    void gdt_flush();
    GDT::Descriptor gGDTPointer;
}
namespace GDT
{
    Entry gGDTEntries[3];

    void SetGate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
    {
        Entry& curr = gGDTEntries[num];
        
        /* Setup the descriptor base address */
        curr.BaseLow = (base & 0xFFFF);
        curr.BaseMid = (base >> 16) & 0xFF;
        curr.BaseHigh = (base >> 24) & 0xFF;
        
        /* Setup the descriptor limits */
        curr.LimitLow = (limit & 0xFFFF);
        curr.Granularity = ((limit >> 16) & 0x0F);
        
        /* Finally, set up the granularity and access flags */
        curr.Granularity |= (gran & 0xF0);
        curr.AccessBits = access;
    }

    void Initialize()
    {
        /* Setup the GDT pointer and limit */
        gGDTPointer.Limit = (uint16_t)0xFFFFFF;
        gGDTPointer.Base = (uint32_t)(gGDTEntries);
        
        /* Our NULL descriptor */
        SetGate(0, 0, 0, 0, 0);
        
        /**
         * The second entry is our Code Segment. The base address
         * is 0, the limit is 4GBytes, it uses 4KByte granularity,
         * uses 32-bit opcodes, and is a Code Segment descriptor.
         */
        SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
        
        /**
         * The third entry is our Data Segment. It's EXACTLY the
         * same as our code segment, but the descriptor type in
         * this entry's access byte says it's a Data Segment
         */
        SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
        
        /* Flush out the old GDT and install the new changes! */
        gdt_flush();
    }
}
