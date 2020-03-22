//
//  panic.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <panic.h>
#include <console.h>

void Panic::Halt(const char* reason)
{
    Console::Write("panic(): ");
    Console::Write(reason);
    Console::Write("\nhalting execution");
    asm("hlt");
}
