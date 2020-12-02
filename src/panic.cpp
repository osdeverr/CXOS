//
//  panic.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <panic.hpp>
#include <stack_trace.hpp>
#include <printf.h>

void cx::os::kernel::debug::TriggerKernelPanic(const char* reason)
{
    printf("\n\n*** FATAL: panic() - %s\n", reason);
    printf("  - Stack Trace:\n");
    DumpStackTrace();
    
    printf("\nHalting execution.");
}
