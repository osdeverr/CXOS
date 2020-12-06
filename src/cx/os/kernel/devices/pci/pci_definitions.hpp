//
//  pci_definitions.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pci_definitions_h
#define pci_definitions_h
#include <stdint.h>
#include <cx/os/kernel/io/ports.hpp>

namespace cx::os::kernel::pci
{
    using PciBus = uint16_t;
    using PciSlot = uint16_t;
    using PciFunction = uint16_t;
    
    using PciVendorId = uint16_t;
    using PciDeviceId = uint16_t;
    
    static constexpr PciVendorId kInvalidVendorId = 0xFFFF;
    
    enum PciControllerPort : ports::PortId
    {
        kPciControllerPort_CfgAddress = 0xCF8,
        kPciControllerPort_CfgData = 0xCFC,
    };
        
    enum PciHeaderOffset : uint8_t
    {
        kPciHeaderOffset_VendorId = 0,
        kPciHeaderOffset_DeviceId = 2,
        kPciHeaderOffset_HeaderType = 7,
        kPciHeaderOffset_ClassSubClass = 10,
        kPciHeaderOffset_BarAddrBase = 16,
        kPciHeaderOffset_IrqLine = 0x3C,
    };
}

#endif /* pci_definitions_h */
