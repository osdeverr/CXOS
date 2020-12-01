//
//  vga_console.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef vga_console_h
#define vga_console_h
#include <console.hpp>

namespace cx::os::kernel::vga
{
    struct VgaConsoleCell
    {
        uint8_t color;
        
        uint8_t fg : 4;
        uint8_t bg : 4;
    };
    
    enum VgaConsoleColor : uint8_t
    {
        kColorBlack = 0x0,
        kColorBlue,
        kColorGreen,
        kColorCyan,
        kColorRed,
        kColorMagenta,
        kColorBrown,
        kColorLightGray,
        
        kColorBright = 0x8,
        kColorBlink = kColorBright,
        
        kColorDarkGray = kColorBlack | kColorBright,
        kColorLightBlue = kColorBlue | kColorBright,
        kColorLightGreen = kColorGreen | kColorBright,
        kColorLightCyan = kColorCyan | kColorBright,
        kColorLightRed = kColorRed | kColorBright,
        kColorLightMagenta = kColorMagenta | kColorBright,
        kColorYellow = kColorBrown | kColorBright,
        kColorWhite = kColorLightGray | kColorBright,
    };
    
    template<size_t W, size_t H>
    using VgaConsoleFramebuffer = VgaConsoleCell[H][W];
    
    class VgaConsole : public console::IAnsiConsoleDevice
    {
    private:
        static constexpr size_t kConsoleWidth = 80, kConsoleHeight = 25;
        
        using Framebuffer = VgaConsoleFramebuffer<kConsoleWidth, kConsoleHeight>;
        inline static auto& kFramebuffer = *(Framebuffer*) 0xB8000;
        
    public:
        virtual void OnConsoleConnected() override {}
        virtual void OnConsoleDisconnected() override {}
        
        virtual console::ConsoleDeviceProperties GetDeviceProperties() const override;
        virtual void SendCharacter(uint8_t c) override;
        virtual void UpdateCursor(size_t x, size_t y) override;
        virtual void ClearScreen() override;
        
    private:
        size_t _x = 0, _y = 0;
        uint8_t _fg = kColorGreen, _bg = kColorBlack;
    };
}

#endif /* vga_console_h */
