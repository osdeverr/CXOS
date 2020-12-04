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
#include <kprintf.hpp>

#include <intrinsics.h>

extern "C"
{
    using namespace cx::os::kernel::interrupts;
    
    InterruptTableDescriptor cx_idt_anchor;
    extern void cx_idt_impl_load_anchor();
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
    constexpr auto kPic1Base = 32, kPic2Base = kPic1Base + 8;
    
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
    
    void HandleIsrException(const InterruptRegisterState& regs)
    {
        char buf[256] = "";
        sprintf(buf, "Kernel caught CPU interrupt #%d\n", regs.interrupt_number);
        
        cx::os::kernel::debug::TriggerKernelPanic(buf);
    }
    
    void HandleIrqInterrupt(const InterruptRegisterState& regs)
    {
        CX_OS_IRQS_OFF();
        
        auto num = regs.interrupt_number;
        
        if(num >= kPic1Base)
        {
            gInterruptHandlers[num - kPic1Base].HandleInterrupt(regs);
        }
        
        if(num >= kPic1Base)
            ports::WriteB(kInterruptControllerPort_MasterCommand, kInterruptControllerCommand_EndOfInterrupt);
        if(num >= kPic2Base)
            ports::WriteB(kInterruptControllerPort_SlaveCommand, kInterruptControllerCommand_EndOfInterrupt);
        
        CX_OS_IRQS_ON();
    }
}

extern "C"
{
    void cx_idt_binding_isr_handler(const InterruptRegisterState& regs)
    {
        cx::os::kernel::interrupts::detail::HandleIsrException(regs);
    }
    
    void cx_idt_binding_irq_handler(const InterruptRegisterState& regs)
    {
        cx::os::kernel::interrupts::detail::HandleIrqInterrupt(regs);
    }
}

void cx::os::kernel::interrupts::SetupInterruptTable()
{
    using namespace detail;
    
    SetupInterruptController(kPic1Base, kPic2Base);
    
    SetupIsrGates<0>();
    
    // PIC 1
    SetupIrqGates<
        kPic1Base + 0,
        kPic1Base + 1,
        kPic1Base + 2,
        kPic1Base + 3,
        kPic1Base + 4,
        kPic1Base + 5,
        kPic1Base + 6,
        kPic1Base + 7
    >();
    
    // PIC 2
    SetupIrqGates<
        kPic2Base + 0,
        kPic2Base + 1,
        kPic2Base + 2,
        kPic2Base + 3,
        kPic2Base + 4,
        kPic2Base + 5,
        kPic2Base + 6,
        kPic2Base + 7
    >();
    
    cx_idt_anchor.limit = sizeof(gRawInterruptTable) - 1;
    cx_idt_anchor.base = (uint32_t) &gRawInterruptTable;
    cx_idt_impl_load_anchor();
}

cx::os::kernel::interrupts::InterruptHandlerKey cx::os::kernel::interrupts::AddIrqHandler(IrqType type, InterruptHandlerPtr handler)
{
    using namespace detail;
    return gInterruptHandlers[int(type)].AddHandler(handler);
}

void cx::os::kernel::interrupts::RemoveIrqHandler(IrqType type, InterruptHandlerKey key)
{
    
}
