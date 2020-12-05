//
//  interrupt_regs.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupt_regs_h
#define interrupt_regs_h
#include <stdint.h>

namespace cx::os::kernel::interrupts
{
    struct InterruptRegisterState
    {
        uint32_t gs, fs, es, ds;
        uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
        uint32_t interrupt_number, error_code;
        uint32_t eip, cs, eflags, user_esp, ss;
    };
}

#endif /* interrupt_regs_h */
