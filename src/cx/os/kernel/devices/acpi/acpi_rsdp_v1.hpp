//
//  acpi_rsdp_v1.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_rsdp_v1_h
#define acpi_rsdp_v1_h
#include <cx/os/kernel/devices/acpi/acpi_validatable.hpp>
#include <cx/os/kernel/devices/acpi/acpi_pointers.hpp>
#include <cx/os/kernel/devices/acpi/acpi_rsdt.hpp>
#include <stdint.h>

namespace cx::os::kernel::acpi
{
    static constexpr auto AcpiRsdpValidSignature = "RSD PTR ";
    
    struct AcpiRsdpV1 : AcpiValidatable<AcpiRsdpV1>
    {
        char signature[8]; // MUST be 'RSD PTR '
        uint8_t checksum_add;
        char oem_id[6];
        uint8_t acpi_revision;
        Acpi32BitPointer<AcpiRsdt> rsdt;
    };
}

#endif /* acpi_rsdp_v1_h */
