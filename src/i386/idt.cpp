//
//  idt.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <idt.h>
#include <ioports.h>
#include <console.h>
#include <itoa.h>

extern "C"
{
    IDT::Descriptor gIDTPointer;
    
    // spam
    extern void irq0();
    extern void irq1();
    extern void irq2();
    extern void irq3();
    extern void irq4();
    extern void irq5();
    extern void irq6();
    extern void irq7();
    extern void irq8();
    extern void irq9();
    extern void irq10();
    extern void irq11();
    extern void irq12();
    extern void irq13();
    extern void irq14();
    extern void irq15();
    extern void isr0();
    extern void isr1();
    extern void isr2();
    extern void isr3();
    extern void isr4();
    extern void isr5();
    extern void isr6();
    extern void isr7();
    extern void isr8();
    extern void isr9();
    extern void isr10();
    extern void isr11();
    extern void isr12();
    extern void isr13();
    extern void isr14();
    extern void isr15();
    extern void isr16();
    extern void isr17();
    extern void isr18();
    extern void isr19();
    extern void isr20();
    extern void isr21();
    extern void isr22();
    extern void isr23();
    extern void isr24();
    extern void isr25();
    extern void isr26();
    extern void isr27();
    extern void isr28();
    extern void isr29();
    extern void isr30();
    extern void isr31();
    
    extern void idt_load();
}
namespace IDT
{
    Entry gIDT[256];
    HandlerList gInterruptHandlers[256];
    constexpr int kIRQBase = 32;
    constexpr uint16_t kPICMaster = 0x20, kPICSlave = 0xA0;
    constexpr uint8_t kEOI = 0x20, kICW1 = 0x11, kICW4 = 0x01;
    
    void InitPIC(uint8_t pic1, uint8_t pic2)
    {
        IOPorts::Out<uint8_t>(kPICMaster, kICW1);
        IOPorts::Out<uint8_t>(kPICSlave, kICW1);
        IOPorts::Out<uint8_t>(kPICMaster + 1, pic1);
        IOPorts::Out<uint8_t>(kPICSlave + 1, pic2);
        IOPorts::Out<uint8_t>(kPICMaster + 1, 4);
        IOPorts::Out<uint8_t>(kPICSlave + 1, 2);
        IOPorts::Out<uint8_t>(kPICMaster + 1, kICW4);
        IOPorts::Out<uint8_t>(kPICSlave + 1, kICW4);
        IOPorts::Out<uint8_t>(kPICMaster + 1, 0xFF);
        
        IOPorts::Out<uint8_t>(kPICMaster + 1, 0x0);
        IOPorts::Out<uint8_t>(kPICSlave + 1, 0x0);
    }
    
    void SetGate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags)
    {
        gIDT[num].BaseLow = (base & 0xFFFF);
        gIDT[num].BaseHigh = (base >> 16) & 0xFFFF;
        
        gIDT[num].Selector = sel;
        gIDT[num].AlwaysZero = 0;
        gIDT[num].Flags = flags;
    }
    
    void OnInterrupt(const RegisterState& regs)
    {
        gInterruptHandlers[regs.int_no].Handle(regs);
        
        if(regs.int_no >= kIRQBase)
        {
            if(regs.int_no >= kIRQBase + 8)
                IOPorts::Out<uint8_t>(kPICSlave, kEOI);
                
            IOPorts::Out<uint8_t>(kPICMaster, kEOI);
            asm("sti");
        }
    }
}

void IDT::Initialize()
{
    InitPIC(0x20, 0x28);
    
    for(int i = 0; i < 256; i++)
        gIDT[i] = {0};
    
    SetGate(   0, (unsigned)isr0,  0x08, 0x8E);
    SetGate(   1, (unsigned)isr1,  0x08, 0x8E);
    SetGate(   2, (unsigned)isr2,  0x08, 0x8E);
    SetGate(   3, (unsigned)isr3,  0x08, 0x8E);
    SetGate(   4, (unsigned)isr4,  0x08, 0x8E);
    SetGate(   5, (unsigned)isr5,  0x08, 0x8E);
    SetGate(   6, (unsigned)isr6,  0x08, 0x8E);
    SetGate(   7, (unsigned)isr7,  0x08, 0x8E);
    SetGate(   8, (unsigned)isr8,  0x08, 0x8E);
    SetGate(   9, (unsigned)isr9,  0x08, 0x8E);
    SetGate(  10, (unsigned)isr10, 0x08, 0x8E);
    SetGate(  11, (unsigned)isr11, 0x08, 0x8E);
    SetGate(  12, (unsigned)isr12, 0x08, 0x8E);
    SetGate(  13, (unsigned)isr13, 0x08, 0x8E);
    SetGate(  14, (unsigned)isr14, 0x08, 0x8E);
    SetGate(  15, (unsigned)isr15, 0x08, 0x8E);
    SetGate(  16, (unsigned)isr16, 0x08, 0x8E);
    SetGate(  17, (unsigned)isr17, 0x08, 0x8E);
    SetGate(  18, (unsigned)isr18, 0x08, 0x8E);
    SetGate(  19, (unsigned)isr19, 0x08, 0x8E);
    SetGate(  20, (unsigned)isr20, 0x08, 0x8E);
    SetGate(  21, (unsigned)isr21, 0x08, 0x8E);
    SetGate(  22, (unsigned)isr22, 0x08, 0x8E);
    SetGate(  23, (unsigned)isr23, 0x08, 0x8E);
    SetGate(  24, (unsigned)isr24, 0x08, 0x8E);
    SetGate(  25, (unsigned)isr25, 0x08, 0x8E);
    SetGate(  26, (unsigned)isr26, 0x08, 0x8E);
    SetGate(  27, (unsigned)isr27, 0x08, 0x8E);
    SetGate(  28, (unsigned)isr28, 0x08, 0x8E);
    SetGate(  29, (unsigned)isr29, 0x08, 0x8E);
    SetGate(  30, (unsigned)isr30, 0x08, 0x8E);
    SetGate(  31, (unsigned)isr31, 0x08, 0x8E);
    
    SetGate(32, (unsigned)irq0,  0x08, 0x8E);
    SetGate(33, (unsigned)irq1,  0x08, 0x8E);
    SetGate(34, (unsigned)irq2,  0x08, 0x8E);
    SetGate(35, (unsigned)irq3,  0x08, 0x8E);
    SetGate(36, (unsigned)irq4,  0x08, 0x8E);
    SetGate(37, (unsigned)irq5,  0x08, 0x8E);
    SetGate(38, (unsigned)irq6,  0x08, 0x8E);
    SetGate(39, (unsigned)irq7,  0x08, 0x8E);
    SetGate(40, (unsigned)irq8,  0x08, 0x8E);
    SetGate(41, (unsigned)irq9,  0x08, 0x8E);
    SetGate(42, (unsigned)irq10, 0x08, 0x8E);
    SetGate(43, (unsigned)irq11, 0x08, 0x8E);
    SetGate(44, (unsigned)irq12, 0x08, 0x8E);
    SetGate(45, (unsigned)irq13, 0x08, 0x8E);
    SetGate(46, (unsigned)irq14, 0x08, 0x8E);
    SetGate(47, (unsigned)irq15, 0x08, 0x8E);
    
    gIDTPointer.Limit = (sizeof (Entry) * 256) - 1;
    gIDTPointer.Base = (uint32_t)&gIDT;
    idt_load();
    
    // enable IRQs
    asm("sti");
}

void IDT::AddIRQHandler(const eIRQ type, const InterruptHandler handler)
{
    gInterruptHandlers[type + kIRQBase].Add(handler);
}
void IDT::AddFaultHandler(const eFault type, const InterruptHandler handler)
{
    gInterruptHandlers[type].Add(handler);
}

extern "C" void IDT_CBinding_InterruptHandler(const IDT::RegisterState& regs)
{
    IDT::OnInterrupt(regs);
}
