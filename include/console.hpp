//
//  console.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef console_h
#define console_h
#include <stdint.h>
#include <stddef.h>

namespace cx::os::kernel::console
{
    struct ConsoleDeviceProperties
    {
        size_t width, height;
        bool supports_colors;
        bool has_cursor;
    };
    
    template<class CharT>
    class IConsoleDevice
    {
    public:
        virtual ~IConsoleDevice() = default;
        
        virtual void OnConsoleConnected() = 0;
        virtual void OnConsoleDisconnected() = 0;
        
        virtual ConsoleDeviceProperties GetDeviceProperties() const = 0;
        virtual void SendCharacter(CharT c) = 0;
        virtual void UpdateCursor(size_t x, size_t y) = 0;
        virtual void ClearScreen() = 0;
    };
    using IAnsiConsoleDevice = IConsoleDevice<uint8_t>;
    
    void SetupAnsiConsole(IAnsiConsoleDevice* output);
    void AnsiPutChar(uint8_t c);
}

#endif /* console_h */
