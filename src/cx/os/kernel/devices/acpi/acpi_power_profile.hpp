//
//  acpi_power_profile.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_power_profile_h
#define acpi_power_profile_h
#include <stdint.h>

namespace cx::os::kernel::acpi
{
    enum class AcpiPowerProfile : uint8_t
    {
        Unspecified,
        Desktop,
        Mobile,
        Workstation,
        EnterpriseServer,
        SohoServer,
        AppliancePC,
        PerformanceServer
    };
}

#endif /* acpi_power_profile_h */
