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
    if(_x >= kConsoleWidth && _y < kConsoleHeight)
    {
        _x = 0;
        _y++;
    }
    
    UpdateCursor(_x, _y);
}

void cx::os::kernel::vga::VgaConsole::UpdateCursor(size_t x, size_t y)
{
    uint16_t pos = y * kConsoleWidth + x;
     
    ports::WriteB(0x3D4, 0x0F);
    ports::WriteB(0x3D5, (uint8_t) (pos & 0xFF));
    ports::WriteB(0x3D4, 0x0E);
    ports::WriteB(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void cx::os::kernel::vga::VgaConsole::ClearScreen()
{
    for(auto& rows : kFramebuffer)
        for(auto& c : rows)
            c = {' ', kColorLightGray, kColorBlack};
}
