//
//  acpi_facs.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_facs_h
#define acpi_facs_h
#include <cx/os/kernel/devices/acpi/acpi_sdt_base.hpp>
#include <cx/os/kernel/devices/acpi/acpi_pointers.hpp>

namespace cx::os::kernel::acpi
{
    struct AcpiFacsFlags
    {
        bool supports_s4bios_req : 1;
        bool supports_64bit_wake : 1;
        int reserved : 30;
    };
    
    struct AcpiOspmFlags
    {
        bool supports_s4bios_req : 1;
        bool supports_64bit_wake : 1;
        int reserved : 31;
    };
    
    struct AcpiGlobalLock
    {
        bool ownership_pending : 1;
        bool owned : 1;
        int reserved : 30;
    };
    
    struct AcpiFacs
    {
        char signature[4]; // FACS
        uint32_t length;
        uint32_t hardware_signature;
        Acpi32BitPointer<void()> hardware_wakeup_addr;
        AcpiGlobalLock global_lock;
        AcpiFacsFlags flags;
        Acpi64BitPointer<void()> x_hardware_wakeup_addr;
        uint8_t version;
        uint8_t reserved[3];
        AcpiOspmFlags ospm_flags;
        uint8_t reserved_2[24];
    };
}

#endif /* acpi_facs_h */
