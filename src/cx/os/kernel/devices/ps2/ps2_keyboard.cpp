//
//  ps2_keyboard.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 05.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/devices/ps2/ps2_keyboard.hpp>
#include <cx/os/kernel/devices/ps2/ps2_protocol.hpp>
#include <cx/os/kernel/interrupts/interrupts.hpp>
#include <cx/os/kernel/kprintf.hpp>
#include <cx/util/round_queue.hpp>

namespace cx::os::kernel::ps2::detail
{
    // Scancode resolution tables copied from U365
    
    constexpr Ps2KeyboardScancodeResolutionPair kScancodesLowercase[] =
    {
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
        {0x52, '0'}, {0x53, '.'}
    };
    
    constexpr Ps2KeyboardScancodeResolutionPair kScancodesUppercase[] =
    {
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
        {0x52, '0'}, {0x53, '.'}
    };
    
    constexpr Ps2KeyboardScancodeResolutionPair kScancodesShift[] =
    {
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
        {0x52, '0'}, {0x53, '.'}
    };
    
    Ps2GlobalState gTheKeyboardState = {};
    bool gKeyboardIrqFired = false;
    
    RoundQueue<Ps2KeyboardEvent, 256> gKeyboardEvents = {};
}

void cx::os::kernel::ps2::InitializePs2Keyboard()
{
    using namespace detail;
    
    interrupts::AddIrqHandler(interrupts::IrqType::PS2Keyboard,
                              [](const interrupts::InterruptRegisterState&)
                              {
                                  auto raw_sc = ports::ReadB(kPs2ControllerPort_Data);
                                  auto sc = raw_sc &~ kPs2KeyboardScancodeFlags_KeyReleased;
                                  auto released = raw_sc & kPs2KeyboardScancodeFlags_KeyReleased;

                                  Ps2KeyboardEvent event = {true, (bool) released, (Ps2KeyboardScancode) sc};
                                  gKeyboardEvents.push(event);
                                  
                                  switch(sc)
                                  {
                                      case kPs2KeyboardSpecialKey_LeftShift:
                                          gTheKeyboardState.left_shift = !released;
                                          break;
                                      case kPs2KeyboardSpecialKey_RightShift:
                                          gTheKeyboardState.right_shift = !released;
                                          break;
                                      case kPs2KeyboardSpecialKey_LeftCtrl:
                                          gTheKeyboardState.left_ctrl = !released;
                                          break;
                                      case kPs2KeyboardSpecialKey_RightCtrl:
                                          gTheKeyboardState.right_ctrl = !released;
                                          break;
                                  };
                                  
                                  gKeyboardIrqFired = true;
                              }
                              );
}

char cx::os::kernel::ps2::ResolvePs2KeyScancode(Ps2KeyboardScancode code)
{
    for(auto& pair : detail::kScancodesLowercase)
        if(pair.first == code)
            return pair.second;
    
    return 0;
}

void cx::os::kernel::ps2::WaitForPs2KeyboardInterrupt()
{
    using namespace detail;
    
    while(!gKeyboardIrqFired)
    {}
    
    gKeyboardIrqFired = false;
}

cx::os::kernel::ps2::Ps2KeyboardEvent cx::os::kernel::ps2::PopPs2KeyboardEvent()
{
    using namespace detail;
    
    while(gKeyboardEvents.empty())
    {}
    
    return gKeyboardEvents.pop();
}

char cx::os::kernel::ps2::Ps2KeyboardGetChar()
{
    using namespace detail;
    
    auto event = PopPs2KeyboardEvent();
    while(event.released)
    {
        WaitForPs2KeyboardInterrupt();
        event = PopPs2KeyboardEvent();
    }
    
    auto resolve =
    [](const auto& event, auto& table) -> char
    {
        for(auto& pair : table)
            if(pair.first == event.code)
                return pair.second;
        
        return 0;
    };
    
    if(gTheKeyboardState.left_shift || gTheKeyboardState.right_shift)
        return resolve(event, kScancodesShift);
    else
        return resolve(event, kScancodesLowercase);
}
