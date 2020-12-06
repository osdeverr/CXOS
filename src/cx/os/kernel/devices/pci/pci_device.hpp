//
//  pci_device.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pci_device_h
#define pci_device_h
#include <cx/os/kernel/devices/pci/pci_definitions.hpp>
#include <cx/os/kernel/devices/pci/pci_subclass.hpp>
#include <cx/os/kernel/devices/pci/pci_class.hpp>

namespace cx::os::kernel::pci
{
    struct PciDevice {
        PciVendorId vendor_id;
        PciDeviceId device_id;
        
        PciClass class_id;
        PciSubClass subclass_id;
    };
}

#endif /* pci_device_h */
