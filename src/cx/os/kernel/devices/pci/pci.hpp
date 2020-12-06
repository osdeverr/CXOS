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
#include <cx/os/kernel/devices/pci/pci_hdr_type.hpp>
#include <cx/os/kernel/devices/pci/pci_bar.hpp>
#include <cx/os/kernel/interrupts/interrupt_types.hpp>

#include <stddef.h>

namespace cx::os::kernel::pci
{
    PciVendorId GetPciDeviceVendor(PciBus bus, PciSlot slot, PciFunction function = 0);
    PciDeviceId GetPciDeviceId(PciBus bus, PciSlot slot, PciFunction function = 0);
    PciDevType  GetPciDeviceType(PciBus bus, PciSlot slot, PciFunction function = 0);
    PciHdrType  GetPciDeviceHdrType(PciBus bus, PciSlot slot, PciFunction function = 0);
    PciBar      GetPciDeviceBar(PciBus bus, PciSlot slot, PciFunction function, size_t bar_id);
    
    interrupts::IrqType GetPciDeviceIrqLine(PciBus bus, PciSlot slot, PciFunction function = 0);
}

#endif /* pci_h */
