//
//  interrupt_table.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupt_table_h
#define interrupt_table_h
#include <stdint.h>

namespace cx::os::kernel::interrupts
{
    struct InterruptTableEntry
    {
        uint16_t base_low;
        uint16_t selector;
        uint8_t always_zero;
        uint8_t flags;
        uint16_t base_high;
    } __attribute__((packed));
    
    struct InterruptTableDescriptor
    {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed));
    
    
}

#endif /* interrupt_table_h */
