//
//  acpi_rsdt.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_rsdt_h
#define acpi_rsdt_h
#include <cx/os/kernel/devices/acpi/acpi_sdt_base.hpp>

namespace cx::os::kernel::acpi
{
    struct AcpiRsdt : AcpiSdtBase, AcpiValidatable<AcpiRsdt>
    {
        static constexpr auto AcpiSdtSignature = "RSDT";
    };
}

#endif /* acpi_rsdt_h */
