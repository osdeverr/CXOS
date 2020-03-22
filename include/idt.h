//
//  interrupts.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupts_h
#define interrupts_h
#include <stdint.h>

namespace IDT
{
    enum eFault
    {
        eFault_DivisionByZero,
        eFault_Debug,
        eFault_NonMaskable,
        eFault_Breakpoint,
        eFault_IntoOverflow,
        eFault_OutOfBounds,
        eFault_InvalidOpcode,
        eFault_NoFPU,
        eFault_DoubleFault,
        eFault_CoprocessorOverrun,
        eFault_InvalidTSS,
        eFault_SegmentNotPresent,
        eFault_StackFault,
        eFault_GPF,
        eFault_PageFault,
        eFault_UnknownInterrupt,
        eFault_FPUError,
        eFault_WrongAlignment,
        eFault_MachineCheck
    };
    enum eIRQ
    {
        eIRQ_PIT,
        eIRQ_PS2Keyboard,
        eIRQ_SlavePIC,
        eIRQ_COM2_4,
        eIRQ_COM1_3,
        eIRQ_LPT2_3_or_SoundCard,
        eIRQ_FloppyDisk,
        eIRQ_LPT1,
        
        // PIC2
        eIRQ_RTC,
        eIRQ_ACPI,
        eIRQ_Peripherals1,
        eIRQ_Peripherals2,
        eIRQ_PS2Mouse,
        eIRQ_Coprocessor,
        eIRQ_ATAMaster,
        eIRQ_ATASlave
    };
    struct Entry
    {
        uint16_t BaseLow;
        uint16_t Selector;
        uint8_t AlwaysZero;
        uint8_t Flags;
        uint16_t BaseHigh;
    } __attribute__((packed));
    
    struct RegisterState
    {
        uint32_t gs,fs,es,ds;
        uint32_t edi,esi,ebp,esp,ebx,edx,ecx,eax;
        uint32_t int_no,err_code;
        uint32_t eip,cs,eflags,useresp,ss;
    };

    typedef void(*InterruptHandler)(const RegisterState&);
    class HandlerList
    {
    public:
        HandlerList() : Count(0) {}
        void Add(const InterruptHandler h) { Handlers[Count++] = h; }
        void Handle(const RegisterState& regs)
        {
            for(uint32_t i = 0; i < Count; i++)
                Handlers[i](regs);
        }
        
    private:
        InterruptHandler Handlers[32];
        uint32_t Count;
    };
    
    struct Descriptor
    {
        uint16_t Limit;
        uint32_t Base;
    } __attribute__((packed));
    
    void Initialize();
    void AddIRQHandler(const eIRQ type, const InterruptHandler handler);
    void AddFaultHandler(const eFault type, const InterruptHandler handler);
}

#endif /* interrupts_h */
