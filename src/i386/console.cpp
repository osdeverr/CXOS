//
//  console.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <console.h>
#include <i386/ioports.h>

namespace VGAConsoleImpl
{
    constexpr int kMaxW = 80, kMaxH = 25;
    int posX = 0, posY = 0;
    
    struct VGAConsoleEntry
    {
        uint8_t character;
        uint8_t attribs;
    };
    constexpr uint8_t kDefaultColor = 0x07;
    constexpr VGAConsoleEntry kEmptyEntry = { ' ', kDefaultColor };
    VGAConsoleEntry* kConsoleBuffer = (VGAConsoleEntry*) 0xB8000;
    
    void SetCursorPos(const int x, const int y)
    {
        uint16_t pos = y * kMaxW + x;
        
        IOPorts::Out<uint8_t>(0x3D4, 0x0F);
        IOPorts::Out<uint8_t>(0x3D5, (uint8_t) (pos & 0xFF));
        IOPorts::Out<uint8_t>(0x3D4, 0x0E);
        IOPorts::Out<uint8_t>(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
    }
    void UpdateCursor()
    {
        SetCursorPos(posX, posY);
    }
    void IncCursorPos()
    {
        posX++;
        if(posX >= kMaxW)
        {
            posY++;
            posX = 0;
        }
    }
    void DecCursorPos()
    {
        posX--;
        if(posX < 0)
        {
            posY--;
            posX = 0;
        }
    }
    void PutChar(const uint8_t c)
    {
        switch(c)
        {
            case '\b':
                DecCursorPos(); return;
            case '\n':
                posX = 0; posY++; return;
            case '\r':
                posX = 0; return;
            default:
                kConsoleBuffer[posY * kMaxW + posX] = { c, kDefaultColor };
                IncCursorPos(); return;
        }
    }
}

void Console::Clear()
{
    using namespace VGAConsoleImpl;
    
    for(int i = 0; i < kMaxW * kMaxH; i++)
        kConsoleBuffer[i] = kEmptyEntry;
    SetCursorPos(0, 0);
}
void Console::Initialize()
{
    Clear();
}
void Console::Write(const uint8_t c)
{
    VGAConsoleImpl::PutChar(c);
    VGAConsoleImpl::UpdateCursor();
}
void Console::Write(const char* str)
{
    while(*str)
    {
        VGAConsoleImpl::PutChar((uint8_t) *(str++));
    }
    VGAConsoleImpl::UpdateCursor();
}
