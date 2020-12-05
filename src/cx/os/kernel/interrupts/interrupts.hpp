//
//  interrupts.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupts_h
#define interrupts_h
#include <cx/os/kernel/interrupts/interrupt_types.hpp>
#include <cx/os/kernel/interrupts/interrupt_handlers.hpp>

namespace cx::os::kernel::interrupts
{
    void SetupInterruptTable();
    
    InterruptHandlerKey AddIrqHandler(IrqType type, InterruptHandlerPtr handler);
    void RemoveIrqHandler(IrqType type, InterruptHandlerKey key);
}

#endif /* interrupts_h */
