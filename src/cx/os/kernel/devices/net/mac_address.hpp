//
//  mac_address.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef mac_address_h
#define mac_address_h
#include <stdint.h>

namespace cx::os::kernel::net
{
    using MacAddress = uint8_t[8];
}

#endif /* mac_address_h */
