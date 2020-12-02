//
//  stack_trace.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef stack_trace_h
#define stack_trace_h

namespace cx::os::kernel::debug
{
    struct DebugStackFrame
    {
        DebugStackFrame* prev;
        void* addr;
    };
    
    void DumpStackTrace();
}

#endif /* stack_trace_h */
