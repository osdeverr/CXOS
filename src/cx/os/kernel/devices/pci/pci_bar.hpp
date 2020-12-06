//
//  pci_bar.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pci_bar_h
#define pci_bar_h
#include <stdint.h>

namespace cx::os::kernel::pci
{
    enum class PciBarType
    {
        Invalid,
        MemoryMapped,
        PortIO
    };
    
    struct PciBar
    {
        PciBarType type = PciBarType::Invalid;
        uint32_t addr = 0;
    };
}

#endif /* pci_bar_h */
