#pragma once

#include <stdint.h>

namespace GDT
{
    struct Entry
    {
        uint16_t LimitLow;
        uint16_t BaseLow;
        uint8_t  BaseMid;
        uint8_t  AccessBits;
        uint8_t  Granularity;
        uint8_t  BaseHigh;
    } __attribute__((packed));

    struct Descriptor
    {
        uint16_t Limit;
        uint32_t Base;
    } __attribute__((packed));
    
    void        Initialize();
    void        SetGate(int, uint32_t, uint32_t, uint8_t, uint8_t);
}
