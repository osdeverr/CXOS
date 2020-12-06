//
//  acpi_sdt_base.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_sdt_base_h
#define acpi_sdt_base_h
#include <cx/os/kernel/devices/acpi/acpi_validatable.hpp>

namespace cx::os::kernel::acpi
{
    struct AcpiSdtBase : AcpiValidatable<AcpiSdtBase>
    {
        char signature[4];
        uint32_t length;
        
        // Overriding it in this case
        bool IsValid() const
        {
            auto begin = reinterpret_cast<const uint8_t*>(this);
            
            uint8_t sum = 0;
            for(auto ptr = begin; ptr < begin + this->length; ptr++)
                sum += *ptr;
            
            return sum == 0;
        }
    };
}

#endif /* acpi_sdt_base_h */
