//
//  panic.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef panic_h
#define panic_h

namespace cx::os::kernel::debug
{
    void TriggerKernelPanic(const char* reason = "<unspecified>");
}

#endif /* panic_h */
