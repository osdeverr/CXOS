//
//  panic.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <cx/os/kernel/debug/panic.hpp>
#include <cx/os/kernel/debug/stack_trace.hpp>
#include <printf.h>
#include <cx/os/kernel/intrinsics.h>

void cx::os::kernel::debug::TriggerKernelPanic(const char* reason)
{
    printf("\n\n*** FATAL: panic() - %s\n", reason);
    printf("  - Stack Trace:\n");
    DumpStackTrace();
    
    printf("\nHalting execution.");
    
    while(true)
    {
        CX_OS_IRQS_OFF();
        CX_OS_HALT_CPU();
    }
}
