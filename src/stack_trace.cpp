//
//  stack_trace.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <stack_trace.hpp>
#include <printf.h>

void cx::os::kernel::debug::DumpStackTrace()
{
    constexpr auto kMaxFrames = 30;
    
    DebugStackFrame* stack;
    asm ("movl %%ebp,%0" : "=r"(stack));
    
    int counter = 0;
    int total = 0;
    for(; stack != nullptr; total++)
    {
        if(counter < kMaxFrames)
        {
            printf("    Instruction @ 0x%08X\n", stack->addr);
            counter++;
        }
        stack = stack->prev;
    }
    
    if(counter < kMaxFrames)
        printf("    <end of trace>\n");
    else
        printf("    <%d more>\n", total - counter);
}
