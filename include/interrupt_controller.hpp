//
//  interrupt_controller.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupt_controller_h
#define interrupt_controller_h
#include <stdint.h>
#include <ports.hpp>

namespace cx::os::kernel::interrupts
{
    enum InterruptControllerPort : ports::PortId
    {
        kInterruptControllerPort_MasterCommand = 0x20,
        kInterruptControllerPort_MasterData,
        
        kInterruptControllerPort_SlaveCommand = 0xA0,
        kInterruptControllerPort_SlaveData,
    };
    
    enum InterruptControllerCommand : uint8_t
    {
        kInterruptControllerCommand_InitCmdWord1 = 0x11,
        kInterruptControllerCommand_InitCmdWord4 = 0x01,
        
        kInterruptControllerCommand_EndOfInterrupt = 0x20,
    };
}

#endif /* interrupt_controller_h */
