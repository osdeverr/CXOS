//
//  acpi_dsdt.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_dsdt_h
#define acpi_dsdt_h
#include <cx/os/kernel/devices/acpi/acpi_sdt_base.hpp>
#include <cx/os/kernel/devices/acpi/acpi_pointers.hpp>

namespace cx::os::kernel::acpi
{
    struct AcpiDsdt : AcpiSdtBase
    {
        uint8_t aml_code[1];
        
        static constexpr auto AcpiSdtSignature = "DSDT";
    };
}

#endif /* acpi_dsdt_h */
