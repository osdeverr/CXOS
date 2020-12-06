//
//  pci_dev_type.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pci_dev_type_h
#define pci_dev_type_h
#include <cx/os/kernel/devices/pci/pci_subclass.hpp>
#include <cx/os/kernel/devices/pci/pci_class.hpp>

namespace cx::os::kernel::pci
{
    struct PciDevType
    {
        PciClass dev_class;
        PciSubClass dev_subclass;
    };
}

#endif /* pci_dev_type_h */
