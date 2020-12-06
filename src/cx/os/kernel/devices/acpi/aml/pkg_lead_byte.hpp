//
//  pkg_lead_byte.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pkg_lead_byte_h
#define pkg_lead_byte_h

namespace cx::os::kernel::acpi::aml
{
    struct PkgLeadByte
    {
        int byte_length : 2;
        int size_if_0byte : 2;
        int lsb_nibble : 4;
    } __attribute__((packed));
}

#endif /* pkg_lead_byte_h */
