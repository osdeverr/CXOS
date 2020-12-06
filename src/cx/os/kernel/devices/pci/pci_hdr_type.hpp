//
//  pci_hdr_type.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pci_hdr_type_h
#define pci_hdr_type_h
#include <stdint.h>

namespace cx::os::kernel::pci
{
    enum PciHdrType : uint8_t
    {
        kPciHdrType_Regular = 0,
        kPciHdrType_Pci2PciBus = 1,
        kPciHdrType_CardBus = 2,
        
        kPciHdrType_MultipleFuncs = 0x80
    };
}

#endif /* pci_hdr_type_h */
