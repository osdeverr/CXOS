//
//  vga_console.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <vga_console.hpp>
#include <ports.hpp>

cx::os::kernel::console::ConsoleDeviceProperties cx::os::kernel::vga::VgaConsole::GetDeviceProperties() const
{
    console::ConsoleDeviceProperties props;
    props.width = kConsoleWidth;
    props.height = kConsoleHeight;
    props.supports_colors = true;
    props.has_cursor = true;
    
    return props;
}

void cx::os::kernel::vga::VgaConsole::SendCharacter(uint8_t c)
{
    switch(c)
    {
        case '\n': _x = kConsoleWidth; break;
        default: kFramebuffer[_y][_x] = {c, _fg, _bg}; break;
    };
    
    _x++;
    if(_x >= kConsoleWidth)
    {
        _x = 0;
        _y++;
    }
    
    if(_y >= kConsoleHeight)
    {
        ScrollDownOneLine();
    }
    
    UpdateCursor(_x, _y);
}

void cx::os::kernel::vga::VgaConsole::UpdateCursor(size_t x, size_t y)
{
    uint16_t pos = y * kConsoleWidth + x;
     
    ports::WriteB(kVgaPort_Command, kVgaPortCmd_UpdateCursorPosX);
    ports::WriteB(kVgaPort_Data, (uint8_t) (pos & 0xFF));
    ports::WriteB(kVgaPort_Command, kVgaPortCmd_UpdateCursorPosY);
    ports::WriteB(kVgaPort_Data, (uint8_t) ((pos >> 8) & 0xFF));
}

void cx::os::kernel::vga::VgaConsole::ClearScreen()
{
    for(auto& rows : kFramebuffer)
        for(auto& c : rows)
            c = {' ', kColorLightGray, kColorBlack};
}

void cx::os::kernel::vga::VgaConsole::ToggleVgaCursor(bool toggle, int begin_scanline, int end_scanline)
{
    ports::WriteB(kVgaPort_Command, kVgaPortCmd_UpdateCursorBegin);
    ports::WriteB(kVgaPort_Data, (ports::ReadB(kVgaPort_Data) & 0xC0) | begin_scanline);
    
    ports::WriteB(kVgaPort_Command, kVgaPortCmd_UpdateCursorEnd);
    ports::WriteB(kVgaPort_Data, (ports::ReadB(kVgaPort_Data) & 0xE0) | end_scanline);
}

void cx::os::kernel::vga::VgaConsole::ScrollDownOneLine()
{
    for(size_t y = 1; y < kConsoleHeight; y++)
        for(size_t x = 0; x < kConsoleWidth; x++)
            kFramebuffer[y - 1][x] = kFramebuffer[y][x];
    
    for(size_t x = 0; x < kConsoleWidth; x++)
        kFramebuffer[kConsoleHeight - 1][x] = {' ', _fg, _bg};
    
    _x = 0;
    _y = kConsoleHeight - 1;
}
