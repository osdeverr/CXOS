//
//  console.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <cx/os/kernel/devices/console/console.hpp>

namespace cx::os::kernel::console::detail
{
    IAnsiConsoleDevice* gOutputDevice = nullptr;
}

void cx::os::kernel::console::SetupAnsiConsole(IAnsiConsoleDevice* output)
{
    using namespace detail;
    
    if(gOutputDevice)
    {
        gOutputDevice->OnConsoleDisconnected();
        gOutputDevice = nullptr;
    }
    
    gOutputDevice = output;
    gOutputDevice->OnConsoleConnected();
}

void cx::os::kernel::console::AnsiPutChar(uint8_t c)
{
    using namespace detail;
    if(gOutputDevice)
        gOutputDevice->SendCharacter(c);
}

// Used by the lightweight printf impl we chose
extern "C" void _putchar(char character)
{
    cx::os::kernel::console::AnsiPutChar(character);
}
