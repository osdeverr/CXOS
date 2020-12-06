//
//  acpi_xsdt.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_xsdt_h
#define acpi_xsdt_h
#include <cx/os/kernel/devices/acpi/acpi_sdt_base.hpp>

namespace cx::os::kernel::acpi
{
    struct AcpiXsdt : AcpiSdtBase, AcpiValidatable<AcpiXsdt>
    {
        static constexpr auto AcpiSdtSignature = "XSDT";
    };
}

#endif /* acpi_xsdt_h */
