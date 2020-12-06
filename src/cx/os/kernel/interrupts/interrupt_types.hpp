//
//  interrupt_types.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupt_types_h
#define interrupt_types_h

namespace cx::os::kernel::interrupts
{
    enum class CpuFaultType
    {
        DivisionByZero,
        Debug,
        NonMaskableInterrupt,
        Breakpoint,
        IntoOverflow,
        OutOfBounds,
        InvalidOpcode,
        NoFPU,
        DoubleFault,
        CoprocessorOverrun,
        InvalidTSS,
        SegmentNotPresent,
        StackFault,
        GeneralProtectionFault,
        PageFault,
        UnknownInterrupt,
        FPUError,
        WrongAlignment,
        MachineStateCheck,
        
        Count
    };
    
    enum class IrqType
    {
        // PIC1
        PIT,
        PS2Keyboard,
        SlavePIC,
        COM2_4,
        COM1_3,
        LPT2_3_or_SoundCard,
        FloppyDisk,
        LPT1,
        
        // PIC2
        RTC,
        ACPI,
        Peripheral1,
        Peripheral2,
        PS2Mouse,
        FPCoprocessor,
        ATAMaster,
        ATASlave,
        
        Count,
        
        NoIrq = 0xFF
    };

    
}

#endif /* interrupt_types_h */
