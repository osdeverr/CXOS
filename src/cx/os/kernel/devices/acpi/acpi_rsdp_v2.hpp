//
//  acpi_rsdp_v2.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_rsdp_v2_h
#define acpi_rsdp_v2_h
#include <cx/os/kernel/devices/acpi/acpi_rsdp_v1.hpp>
#include <cx/os/kernel/devices/acpi/acpi_xsdt.hpp>

namespace cx::os::kernel::acpi
{
    struct AcpiRsdpV2 : AcpiRsdpV1, AcpiValidatable<AcpiRsdpV2>
    {
        uint32_t length;
        Acpi64BitPointer<AcpiXsdt> xsdt;
        uint8_t ext_checksum;
        uint8_t reserved[3];
    };
}

#endif /* acpi_rsdp_v2_h */
