//
//  panic.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef panic_h
#define panic_h
namespace Panic
{
    void Halt(const char* reason = "Unspecified");
}
#define panic Panic::Halt

#endif /* panic_h */
