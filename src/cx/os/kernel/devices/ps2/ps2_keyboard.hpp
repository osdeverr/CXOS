//
//  ps2_keyboard.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 05.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef ps2_keyboard_h
#define ps2_keyboard_h
#include <cx/stl/pair.hpp>
#include <stdint.h>

namespace cx::os::kernel::ps2
{
    using Ps2KeyboardScancode = uint8_t;
    using Ps2KeyboardScancodeResolutionPair = std::pair<Ps2KeyboardScancode, char>;
    
    enum Ps2KeyboardScancodeFlags : Ps2KeyboardScancode
    {
        kPs2KeyboardScancodeFlags_KeyReleased = 0x80
    };
    
    enum Ps2KeyboardSpecialKey : Ps2KeyboardScancode
    {
        kPs2KeyboardSpecialKey_LeftShift = 0x2A,
        kPs2KeyboardSpecialKey_RightShift = 0x36,
        
        kPs2KeyboardSpecialKey_LeftCtrl = 0x1D,
        kPs2KeyboardSpecialKey_RightCtrl = 0x38
    };
    
    struct Ps2GlobalState
    {
        bool left_shift = false;
        bool right_shift = false;
        bool left_ctrl = false;
        bool right_ctrl = false;
        
        bool num_lock = false;
        bool caps_lock = false;
        bool scroll_lock = false;
    };
    
    struct Ps2KeyboardEvent
    {
        bool valid = false;
        bool released = false;
        
        Ps2KeyboardScancode code = 0;
    };
    
    void InitializePs2Keyboard();
    
    char ResolvePs2KeyScancode(Ps2KeyboardScancode code);
    Ps2KeyboardEvent PopPs2KeyboardEvent();
    char Ps2KeyboardGetChar();
    void WaitForPs2KeyboardInterrupt();
}

#endif /* ps2_keyboard_h */
