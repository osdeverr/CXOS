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
        uint8_t  revision;
        uint8_t  prog_if;
        PciSubClass subclass_id;
        PciClass class_id;
        uint8_t  cache_line_size;
        uint8_t  latency_timer;
        uint8_t  hdr_type;
        uint8_t  bist;
        uint32_t bar[6];
        uint32_t cardbus_cis_ptr;
        uint16_t subsys_vendor;
        uint16_t subsys_id;
        uint32_t expansion_rom;
        uint8_t  capatibilities;
        uint8_t  reserved[3];
        uint32_t reserved2;
        uint8_t  int_line;
        uint8_t  int_pin;
        uint8_t  min_grant;
        uint8_t  max_latency;
    };
}

#endif /* pci_device_h */
