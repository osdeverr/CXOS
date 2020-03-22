//
//  ioports.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef ioports_h
#define ioports_h
#include <stdint.h>

namespace IOPorts
{
    typedef uint16_t Port;
    
    template<typename T>
    T In(const Port portID)
    {
        T ret;
        switch(sizeof(T))
        {
            case 1: // single byte inb
                asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(portID) );
                break;
            case 2: // uint16 inw
                asm volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(portID) );
                break;
            case 4: // uint32 inl
                asm volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(portID) );
                break;
            default:
                asm volatile ("int 3");
        }
        return ret;
    }
    
    template<typename T>
    void Out(const Port portID, const T value)
    {
        switch(sizeof(T))
        {
            case 1: // single byte outb
                asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(portID) );
                break;
            case 2: // uint16 outw
                asm volatile ( "outw %0, %1" : : "a"(value), "Nd"(portID) );
                break;
            case 4: // uint32 outl
                asm volatile ( "outl %0, %1" : : "a"(value), "Nd"(portID) );
                break;
            default:
                asm volatile ("int 3");
        }
    }

}

#endif /* ioports_h */
