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
    if(_esc == VgaEscapeState::Idle)
    {
        switch(c)
        {
            case '\n': _x = kConsoleWidth; break;
            case '\e': _esc = VgaEscapeState::WaitingForBracket; return;
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
    else
    {
        auto atoi =
        [](const char *str)
        {
            int res = 0; // Initialize result
            
            // Iterate through all characters of input string and update result
            for (int i = 0; str[i] != '\0'; ++i)
                res = res*10 + str[i] - '0';
            
            // return result.
            return res;
        };
        
        bool done = false;
        
        if(_esc == VgaEscapeState::WaitingForBracket)
        {
            if(c == '[')
                _esc = VgaEscapeState::Reading;
            else
                done = true;
            
            return;
        }
        else if(_esc == VgaEscapeState::Reading)
        {
            if(_escbuf.size() >= _escbuf.capacity())
            {
                done = true;
            }
            else
            {
                if(c == 'm')
                {
                    _esc = VgaEscapeState::Idle;
                    
                    switch(atoi(&_escbuf[0])) //We're using a big switch-case construction there.
                    {
                        case 0:  _fg = kDefaultFg; _bg = kDefaultBg; break;
                            /*
                        case 1:  tty_setcolor((tty_color<8) ? tty_color+0x08 : tty_color); break;
                        case 2:  tty_setcolor((tty_color>8) ? tty_color-0x08 : tty_color); break;
                        case 7:  tty_setcolor(((tty_color << 4) | (tty_color >> 4))); break;
                        case 8:  tty_setcolor(tty_mkcolor(tty_color>>4, tty_color>>4)); break;
                        case 21: tty_setcolor((tty_color>8) ? tty_color-0x08 : tty_color); break;
                        case 22: tty_setcolor((tty_color<8) ? tty_color+0x08 : tty_color); break;
                        case 27: tty_color = tty_mkcolor(tty_color<<4, tty_color>>4); break;
                        case 28: tty_setcolor(TERM_DEFAULT_COLOR); break;
                             */
                            //Color setting, yes, a lot of code.
                            //foreground
                        case 39: _fg = kColorLightGray; break;
                        case 30: _fg = kColorBlack; break;
                        case 31: _fg = kColorRed; break;
                        case 32: _fg = kColorGreen; break;
                        case 33: _fg = kColorBrown; break;
                        case 34: _fg = kColorBlue; break;
                        case 35: _fg = kColorMagenta; break;
                        case 36: _fg = kColorCyan; break;
                        case 37: _fg = kColorLightGray; break;
                        case 90: _fg = kColorDarkGray; break;
                        case 91: _fg = kColorLightRed; break;
                        case 92: _fg = kColorLightGreen; break;
                        case 93: _fg = kColorYellow; break;
                        case 94: _fg = kColorLightBlue; break;
                        case 95: _fg = kColorLightMagenta; break;
                        case 96: _fg = kColorLightCyan; break;
                        case 97: _fg = kColorWhite; break;
                            //background
                        case 49: _bg = kColorBlack; break;
                        case 40: _bg = kColorBlack; break;
                        case 41: _bg = kColorRed; break;
                        case 42: _bg = kColorGreen; break;
                        case 43: _bg = kColorYellow; break;
                        case 44: _bg = kColorBlue; break;
                        case 45: _bg = kColorMagenta; break;
                        case 46: _bg = kColorCyan; break;
                        case 47: _bg = kColorLightGray; break;
                        case 100: _bg = kColorDarkGray; break;
                        case 101: _bg = kColorLightRed; break;
                        case 102: _bg = kColorGreen; break;
                        case 103: _bg = kColorBrown; break;
                        case 104: _bg = kColorLightBlue; break;
                        case 105: _bg = kColorLightMagenta; break;
                        case 106: _bg = kColorLightCyan; break;
                        case 107: _bg = kColorWhite; break;
                    }
                    
                    done = true;
                }
                else
                {
                    _escbuf.push_back(c);
                }
            }
        }
        
        if(done)
        {
            _esc = VgaEscapeState::Idle;
            _escbuf.clear();
        }
    }
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
