//
//  interrupt_handlers.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupt_handlers_h
#define interrupt_handlers_h
#include <interrupt_regs.hpp>
#include <fixed_vector.hpp>

namespace cx::os::kernel::interrupts
{
    using InterruptHandlerPtr = void(*)(const InterruptRegisterState&);
    using InterruptHandlerList = FixedVector<InterruptHandlerPtr, 32>;
    
    class InterruptHandlerCollection
    {
    public:
        void AddHandler(InterruptHandlerPtr handler)
        {
            _handlers.push_back(handler);
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
