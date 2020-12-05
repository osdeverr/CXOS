//
//  ps2_protocol.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 05.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef ps2_protocol_h
#define ps2_protocol_h
#include <ports.hpp>

namespace cx::os::kernel::ps2
{
    enum Ps2ControllerPort
    {
        kPs2ControllerPort_Data = 0x60,
        kPs2ControllerPort_CommandStatus = 0x64,
    };
    
    struct Ps2StatusRegister
    {
        bool output_full : 1;
        bool input_full : 1;
        bool post_passed : 1;
        bool cmd_data : 1;
        bool chipset_reserved_1 : 1;
        bool chipset_reserved_2 : 1;
        bool timeout : 1;
        bool parity_error : 1;
    };
}

#endif /* ps2_protocol_h */
