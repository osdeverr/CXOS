//
//  u64helpers.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <stdint.h>

extern "C" uint64_t __umoddi3(uint64_t a, uint64_t b)
{
    return (uint32_t) a % (uint32_t) b;
}

extern "C" uint64_t __udivdi3(uint64_t a, uint64_t b)
{
    return (uint32_t) a / (uint32_t) b;
}
