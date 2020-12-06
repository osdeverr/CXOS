//
//  acpi_validatable.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_validatable_h
#define acpi_validatable_h
#include <stdint.h>

namespace cx::os::kernel::acpi
{
    template<class T>
    class AcpiValidatable
    {
    public:
        bool IsValid() const
        {
            auto  ptr = reinterpret_cast<const T*>(this);
            auto& array = reinterpret_cast<const uint8_t(&)[sizeof(T)]>(*ptr);
            
            uint8_t sum = 0;
            for(auto& byte : array)
                sum += byte;
            
            return sum == 0;
        }
    };
}

#endif /* acpi_validatable_h */
