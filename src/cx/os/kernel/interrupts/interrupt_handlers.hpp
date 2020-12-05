//
//  interrupt_handlers.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupt_handlers_h
#define interrupt_handlers_h
#include <cx/os/kernel/interrupts/interrupt_regs.hpp>
#include <cx/util/fixed_vector.hpp>
#include <cx/os/kernel/kprintf.hpp>

#include <cx/stl/function.hpp>

namespace cx::os::kernel::interrupts
{
    using InterruptHandlerPtr = std::function<void(const InterruptRegisterState&)>;
    using InterruptHandlerList = FixedVector<InterruptHandlerPtr, 32>;
    using InterruptHandlerKey = void*;
    
    class InterruptHandlerCollection
    {
    public:
        InterruptHandlerKey AddHandler(InterruptHandlerPtr handler)
        {
            _handlers.push_back(handler);
            return *reinterpret_cast<InterruptHandlerKey*>(&handler);
        }
        
        void HandleInterrupt(const InterruptRegisterState& regs)
        {
            for(auto& handler : _handlers)
                handler(regs);
        }
        
    private:
        InterruptHandlerList _handlers;
    };
}

#endif /* interrupt_handlers_h */
