//
//  interrupts.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <interrupts.hpp>
#include <interrupt_table.hpp>
#include <interrupt_regs.hpp>
#include <interrupt_types.hpp>
#include <interrupt_handlers.hpp>
#include <interrupt_controller.hpp>
#include <ports.hpp>
#include <panic.hpp>

#include <printf.h>

extern "C"
{
    using namespace cx::os::kernel::interrupts;
    
    InterruptTableDescriptor cx_idt_anchor;
    extern void cx_idt_impl_load_anchor();
    
    void cx_idt_binding_interrupt_handler(const InterruptRegisterState& regs)
    {
        char buf[256] = "";
        sprintf(buf, "Caught CPU fault #%d\n", regs.interrupt_number);
        
        cx::os::kernel::debug::TriggerKernelPanic(buf);
    }
}

namespace cx::os::kernel::interrupts::detail
{
    template<int InterruptNum>
    __attribute__((naked)) void GenericIsrHandlerStubNoErrcode()
    {
        __asm("cli; push $0; push %0; jmp cx_idt_impl_isr_handler" ::"g"(InterruptNum));
    }
    
    template<int InterruptNum>
    __attribute__((naked)) void GenericIsrHandlerStubErrcode()
    {
        __asm("cli; push %0; jmp cx_idt_impl_isr_handler" ::"g"(InterruptNum));
    }
    
    template<int InterruptNum>
    __attribute__((naked)) void GenericIrqHandlerStub()
    {
        __asm("cli; push $0; push %0; jmp cx_idt_impl_irq_handler" ::"g"(InterruptNum));
    }
    
    constexpr auto kInterruptTableSize = 256;
    
    InterruptTableEntry gRawInterruptTable[kInterruptTableSize] = { 0 };
    InterruptHandlerCollection gInterruptHandlers[int(IrqType::Count)] = {};
    
    void SetGateRaw(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags)
    {
        gRawInterruptTable[num].base_low = (base & 0xFFFF);
        gRawInterruptTable[num].base_high = (base >> 16) & 0xFFFF;
        
        gRawInterruptTable[num].selector = sel;
        gRawInterruptTable[num].always_zero = 0;
        gRawInterruptTable[num].flags = flags;
    }
    
    template<int IntNo>
    void SetupIsrGate()
    {
        SetGateRaw(IntNo, (uint64_t)&GenericIsrHandlerStubNoErrcode<IntNo>, 0x08, 0x8E);
    }
    
    template<int... IntNos>
    void SetupIsrGates()
    {
        (SetupIsrGate<IntNos>() , ...);
    }
    
    template<int IntNo>
    void SetupIsrGateErrCode()
    {
        SetGateRaw(IntNo, (uint64_t)&GenericIsrHandlerStubErrcode<IntNo>, 0x08, 0x8E);
    }
    
    template<int... IntNos>
    void SetupIsrGatesErrCode()
    {
        (SetupIsrGate<IntNos>() , ...);
    }
    
    template<int IntNo>
    void SetupIrqGate()
    {
        SetGateRaw(IntNo, (uint64_t)&GenericIrqHandlerStub<IntNo>, 0x08, 0x8E);
    }
    
    template<int... IntNos>
    void SetupIrqGates()
    {
        (SetupIrqGate<IntNos>() , ...);
    }
    
    void SetupInterruptController(uint8_t pic1_base, uint8_t pic2_base)
    {
        ports::WriteB(kInterruptControllerPort_MasterCommand, kInterruptControllerCommand_InitCmdWord1);
        ports::WriteB(kInterruptControllerPort_SlaveCommand, kInterruptControllerCommand_InitCmdWord1);
        
        ports::WriteB(kInterruptControllerPort_MasterData, pic1_base);
        ports::WriteB(kInterruptControllerPort_SlaveData, pic2_base);
        
        ports::WriteB(kInterruptControllerPort_MasterData, 4);
        ports::WriteB(kInterruptControllerPort_SlaveData, 2);
        
        ports::WriteB(kInterruptControllerPort_MasterCommand, kInterruptControllerCommand_InitCmdWord4);
        ports::WriteB(kInterruptControllerPort_SlaveCommand, kInterruptControllerCommand_InitCmdWord4);
        ports::WriteB(kInterruptControllerPort_MasterData, 0xFF);
        
        ports::WriteB(kInterruptControllerPort_MasterData, 0);
        ports::WriteB(kInterruptControllerPort_SlaveData, 0);
    }
}

void cx::os::kernel::interrupts::SetupInterruptTable()
{
    using namespace detail;
    
    SetupInterruptController(32, 40);
    
    SetupIrqGates<0>();
    
    // PIC 1
    SetupIrqGates<32, 33, 34, 35, 36, 37, 38, 39>();
    
    // PIC 2
    SetupIrqGates<40, 41, 42, 43, 44, 45, 46, 47>();
    
    cx_idt_anchor.limit = sizeof(gRawInterruptTable) - 1;
    cx_idt_anchor.base = (uint32_t) &gRawInterruptTable;
    cx_idt_impl_load_anchor();
}
