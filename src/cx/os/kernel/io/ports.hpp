//
//  ports.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef ports_h
#define ports_h
#include <stdint.h>

namespace cx::os::kernel::ports
{
    using PortId = uint16_t;
    
    // Read ops
    
    inline uint8_t ReadB(PortId id)
    {
        uint8_t result;
        asm volatile ( "inb %1, %0" : "=a"(result) : "Nd"(id) );
        return result;
    }
    
    inline uint16_t ReadW(PortId id)
    {
        uint16_t result;
        asm volatile ( "inw %1, %0" : "=a"(result) : "Nd"(id) );
        return result;
    }
    
    inline uint32_t ReadL(PortId id)
    {
        uint32_t result;
        asm volatile ( "inl %1, %0" : "=a"(result) : "Nd"(id) );
        return result;
    }
    
    // Write ops
    
    inline void WriteB(PortId id, uint8_t value)
    {
        asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(id) );
    }
    
    inline void WriteW(PortId id, uint16_t value)
    {
        asm volatile ( "outw %0, %1" : : "a"(value), "Nd"(id) );
    }
    
    inline void WriteL(PortId id, uint32_t value)
    {
        asm volatile ( "outl %0, %1" : : "a"(value), "Nd"(id) );
    }
}

#endif /* ports_h */
