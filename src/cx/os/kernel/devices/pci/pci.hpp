//
//  pci.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pci_h
#define pci_h
#include <cx/os/kernel/devices/pci/pci_definitions.hpp>
#include <cx/os/kernel/devices/pci/pci_device.hpp>
#include <cx/os/kernel/devices/pci/pci_dev_type.hpp>

namespace cx::os::kernel::pci
{
    PciVendorId GetPciDeviceVendor(PciBus bus, PciSlot slot, PciFunction function);
    PciDeviceId GetPciDeviceId(PciBus bus, PciSlot slot, PciFunction function);
    PciDevType  GetPciDeviceType(PciBus bus, PciSlot slot, PciFunction function);
}

#endif /* pci_h */
