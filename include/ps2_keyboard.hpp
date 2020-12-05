//
//  ps2_keyboard.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 05.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef ps2_keyboard_h
#define ps2_keyboard_h
#include <stl/pair.hpp>
#include <stdint.h>

namespace cx::os::kernel::ps2
{
    using Ps2KeyboardScancode = uint8_t;
    using Ps2KeyboardScancodeResolvementPair = std::pair<Ps2KeyboardScancode, char>;
    
    enum Ps2KeyboardScancodeFlags : Ps2KeyboardScancode
    {
        kPs2KeyboardScancodeFlags_KeyReleased = 0x80
    };
    
    char ResolveKeyScancode(Ps2KeyboardScancode code);
}

#endif /* ps2_keyboard_h */
