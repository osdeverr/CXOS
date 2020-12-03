//
//  interrupts.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <interrupt_table.hpp>
#include <interrupt_regs.hpp>
#include <interrupt_handlers.hpp>

extern "C"
{
    using namespace cx::os::kernel::interrupts;
    
    InterruptTableDescriptor cx_idt_anchor;
    
    void cx_idt_binding_interrupt_handler(const InterruptRegisterState& regs)
    {
        
    }
}

namespace cx::os::kernel::interrupts::detail
{
    template<int InterruptNum>
    __attribute__((naked)) void GenericIrqHandlerStub()
    {
        __asm("cli; push $0; push %0; jmp cx_idt_impl_irq_handler" ::"g"(InterruptNum));
    }
    
    constexpr auto kInterruptTableSize = 256;
    InterruptTableEntry gRawInterruptTable[kInterruptTableSize] = { 0 };
    InterruptHandlerCollection gInterruptHandlers[kInterruptTableSize] = {};
    
    void SetGateRaw(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags)
    {
        gRawInterruptTable[num].base_low = (base & 0xFFFF);
        gRawInterruptTable[num].base_high = (base >> 16) & 0xFFFF;
        
        gRawInterruptTable[num].selector = sel;
        gRawInterruptTable[num].always_zero = 0;
        gRawInterruptTable[num].flags = flags;
    }
    
    template<int IntNo>
    void SetupIrqGate()
    {
        SetGate(IntNo, (uint64_t)&GenericIrqHandlerStub<IntNo>, 0x08, 0x8E);
    }
}
