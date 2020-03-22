//
//  keyboard.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <keyboard.h>
#include <idt.h>
#include <ioports.h>
#include <console.h>

#define KBD_DATA 0x60

#define KEY_ESC         0x1
#define KEY_BACKSPACE   0xE
#define KEY_TAB         0xF

#define KEY_P           0x19
#define KEY_ENTER       0x1C
#define KEY_S           0x1F
#define KEY_LCTRL       0x1D
#define KEY_F           0x21
#define KEY_LSHIFT      0x2A
#define KEY_C           0x2E
#define KEY_CAPSLOCK    0x3A

#define KEY_RSHIFT      0x36
#define KEY_RCTRL       0x38

#define KEY_HOME        0x147
#define KEY_ARROW_UP    0x148
#define KEY_ARROW_LEFT  0x14B
#define KEY_ARROW_RIGHT 0x14D
#define KEY_END         0x14F
#define KEY_ARROW_DOWN  0x150
#define KEY_DELETE      0x153

namespace Keyboard
{
    struct ScancodeCharPair {
        uint8_t scancode;
        char chr; //Character it corresponds to.
    };
    
    ScancodeCharPair regular_scancodes[] = {
        /* Numerical keys */
        {0x02, '1'}, {0x03, '2'}, {0x04, '3'}, {0x05, '4'}, {0x06, '5'}, {0x07, '6'}, {0x08, '7'}, {0x09, '8'}, {0x0A, '9'}, {0x0B, '0'},
        /* Some characters after numerical keys */
        {0x0C, '-'}, {0x0D, '='}, {0x0E, '\b'}, {0x0F, '\t'},
        /* Alphabet! */
        {0x10, 'q'}, {0x11, 'w'}, {0x12, 'e'}, {0x13, 'r'}, {0x14, 't'}, {0x15, 'y'}, {0x16, 'u'}, {0x17, 'i'}, {0x18, 'o'}, {0x19, 'p'}, {0x1A, '['}, {0x1B, ']'}, {0x1C, '\n'},
        {0x1E, 'a'}, {0x1F, 's'}, {0x20, 'd'}, {0x21, 'f'}, {0x22, 'g'}, {0x23, 'h'}, {0x24, 'j'}, {0x25, 'k'}, {0x26, 'l'}, {0x27, ';'}, {0x28, '\''}, {0x29, '`'},
        {0x2B, '\\'}, {0x2C, 'z'}, {0x2D, 'x'}, {0x2E, 'c'}, {0x2F, 'v'}, {0x30, 'b'}, {0x31, 'n'}, {0x32, 'm'}, {0x33, ','}, {0x34, '.'}, {0x35, '/'}, {0x37, '*'},
        {0x39, ' '}, {0x47, '7'}, {0x48, '8'}, {0x49, '9'}, {0x4A, '-'},
        {0x4B, '4'}, {0x4C, '5'}, {0x4D, '6'}, {0x4E, '+'},
        {0x4F, '1'}, {0x50, '2'}, {0x51, '3'},
        {0x52, '0'}, {0x53, '.'}, {0x00, '\0'}
    };
    ScancodeCharPair uppercase_scancodes[] = {
        /* Numerical keys */
        {0x02, '1'}, {0x03, '2'}, {0x04, '3'}, {0x05, '4'}, {0x06, '5'}, {0x07, '6'}, {0x08, '7'}, {0x09, '8'}, {0x0A, '9'}, {0x0B, '0'},
        /* Some characters after numerical keys */
        {0x0C, '-'}, {0x0D, '='}, {0x0E, '\b'}, {0x0F, '\t'},
        /* Alphabet! */
        {0x10, 'Q'}, {0x11, 'W'}, {0x12, 'E'}, {0x13, 'R'}, {0x14, 'T'}, {0x15, 'Y'}, {0x16, 'U'}, {0x17, 'I'}, {0x18, 'O'}, {0x19, 'P'}, {0x1A, '['}, {0x1B, ']'}, {0x1C, '\n'},
        {0x1E, 'A'}, {0x1F, 'S'}, {0x20, 'D'}, {0x21, 'F'}, {0x22, 'G'}, {0x23, 'H'}, {0x24, 'J'}, {0x25, 'K'}, {0x26, 'L'}, {0x27, ';'}, {0x28, '\''}, {0x29, '`'},
        {0x2B, '\\'}, {0x2C, 'Z'}, {0x2D, 'X'}, {0x2E, 'C'}, {0x2F, 'V'}, {0x30, 'B'}, {0x31, 'N'}, {0x32, 'M'}, {0x33, ','}, {0x34, '.'}, {0x35, '/'}, {0x37, '*'},
        {0x39, ' '}, {0x47, '7'}, {0x48, '8'}, {0x49, '9'}, {0x4A, '-'},
        {0x4B, '4'}, {0x4C, '5'}, {0x4D, '6'}, {0x4E, '+'},
        {0x4F, '1'}, {0x50, '2'}, {0x51, '3'},
        {0x52, '0'}, {0x53, '.'}, {0x00, '\0'}
    };
    ScancodeCharPair shift_modified_scancodes[] = {
        /* Numerical keys */
        {0x02, '!'}, {0x03, '@'}, {0x04, '#'}, {0x05, '$'}, {0x06, '%'}, {0x07, '^'}, {0x08, '&'}, {0x09, '*'}, {0x0A, '('}, {0x0B, ')'},
        /* Some characters after numerical keys */
        {0x0C, '_'}, {0x0D, '+'}, {0x0E, '\b'}, {0x0F, '\t'},
        /* Alphabet! */
        {0x10, 'Q'}, {0x11, 'W'}, {0x12, 'E'}, {0x13, 'R'}, {0x14, 'T'}, {0x15, 'Y'}, {0x16, 'U'}, {0x17, 'I'}, {0x18, 'O'}, {0x19, 'P'}, {0x1A, '{'}, {0x1B, '}'}, {0x1C, '\n'},
        {0x1E, 'A'}, {0x1F, 'S'}, {0x20, 'D'}, {0x21, 'F'}, {0x22, 'G'}, {0x23, 'H'}, {0x24, 'J'}, {0x25, 'K'}, {0x26, 'L'}, {0x27, ':'}, {0x28, '"'}, {0x29, '~'},
        {0x2B, '\\'}, {0x2C, 'Z'}, {0x2D, 'X'}, {0x2E, 'C'}, {0x2F, 'V'}, {0x30, 'B'}, {0x31, 'N'}, {0x32, 'M'}, {0x33, '<'}, {0x34, '>'}, {0x35, '?'}, {0x37, '*'},
        {0x39, ' '}, {0x47, '7'}, {0x48, '8'}, {0x49, '9'}, {0x4A, '-'},
        {0x4B, '4'}, {0x4C, '5'}, {0x4D, '6'}, {0x4E, '+'},
        {0x4F, '1'}, {0x50, '2'}, {0x51, '3'},
        {0x52, '0'}, {0x53, '.'}, {0x00, '\0'}
    };
    
    struct Event
    {
        int exists  : 1;
        int release : 1;
        int lshift  : 1;
        int rshift  : 1;
        int lctrl   : 1;
        int rctrl   : 1;
        int numlk   : 1;
        int capslk  : 1;
        int scrllk  : 1;
        int code    : 11;
    } __attribute__((packed));
    
    Event gBuffer[256] = {};
    uint32_t gBufIn  = 0;
    uint32_t gBufOut = 0;
    
    void BufPush(Event value)
    {
        gBuffer[(gBufIn++) % 256] = value;
        if(gBufIn > 256 && gBufOut > 256)
        {
            gBufIn -= 256;
            gBufOut -= 256;
        }
        if(gBufIn - gBufOut > 256)
        {
            // Handle buffer overflow here
            // actually, nothing bad happens
        }
    }
    
    Event BufPop()
    {
        static Event none = {0};
        if(gBufIn <= gBufOut)
            return none;
        return gBuffer[(gBufOut++) % 256];
    }
    
    // Interrupts part of the keyboard driver.
    volatile bool gIRQFired = false;
    void WaitForIRQ()
    {
        //if(kbd_irq_fired){while(kbd_irq_fired);return;}
        while(!gIRQFired);
        gIRQFired = false;
    }
    
    char EventConvert(Event e)
    {
        if(!e.exists || e.release)
            return 0;
        
        uint8_t shift = (e.lshift || e.rshift) && !e.capslk;
        uint8_t uppercase = e.capslk && !e.lshift && !e.rshift;
        
        if(uppercase)
        {
            for(int i=0; uppercase_scancodes[i].scancode != 0x00; i++)
                if(uppercase_scancodes[i].scancode == e.code)
                    return uppercase_scancodes[i].chr;
            return 0;
        }
        else if(shift)
        {
            for(int i=0; shift_modified_scancodes[i].scancode != 0x00; i++)
                if(shift_modified_scancodes[i].scancode == e.code)
                    return shift_modified_scancodes[i].chr;
            return 0;
        }
        else {
            for(int i=0; regular_scancodes[i].scancode != 0x00; i++)
                if(regular_scancodes[i].scancode == e.code)
                    return regular_scancodes[i].chr;
            return 0;
        }
    }
    
    void PS2EventHandler(const IDT::RegisterState&)
    {
        static Event state = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        gIRQFired = true;
        //We need to put every pressed printable key to the buffer.
        uint8_t scancode = IOPorts::In<uint8_t>(KBD_DATA);
        if(scancode > 0x80 && scancode < 0xE0)
        {
            state.release = 1;
            scancode -= 0x80;
        }
        
        switch(scancode)
        {
            case KEY_LSHIFT: state.lshift = !state.release; break;
            case KEY_RSHIFT: state.rshift = !state.release; break;
            case KEY_LCTRL:  state.lctrl  = !state.release; break;
            case KEY_RCTRL:  state.rctrl  = !state.release; break;
            case KEY_CAPSLOCK: if(!state.release) state.capslk = !state.capslk; break;
            case 0xE0: state.code |= 0x100;
                //case 0xE1: state.code |= 0x200;
                //case 0xF0: state.release = 1;
                //default:   state.release = 0;
        }
        
        if(scancode != 0xE0 && scancode != 0xE1)
        {
            state.code += scancode;
            BufPush(state);
            state.code = 0;
            state.release = 0;
        }
    }
    

}

void Keyboard::Initialize()
{
    IDT::AddIRQHandler(IDT::eIRQ_PS2Keyboard, PS2EventHandler);
}
char Keyboard::ReadChar()
{
    char ret = 0;
    while(!ret)
    {
        // If the queue is empty, try to wait, rush B otherwise :)
        if(gBufIn == gBufOut)
        {
            //sleep(1);
            WaitForIRQ();
        }
        ret = EventConvert(BufPop());
    }
    return ret;
}
