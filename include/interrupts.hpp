//
//  interrupts.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef interrupts_h
#define interrupts_h

namespace cx::os::kernel::interrupts
{
    void SetupInterruptTable();
    
    void AddIrqHandler();
    void RemoveIrqHandler();
}

#endif /* interrupts_h */
