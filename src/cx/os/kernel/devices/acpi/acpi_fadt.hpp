//
//  acpi_fadt.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_fadt_h
#define acpi_fadt_h
#include <cx/os/kernel/devices/acpi/acpi_sdt_base.hpp>
#include <cx/os/kernel/devices/acpi/acpi_pointers.hpp>

#include <cx/os/kernel/devices/acpi/acpi_facs.hpp>
#include <cx/os/kernel/devices/acpi/acpi_dsdt.hpp>

#include <cx/os/kernel/devices/acpi/acpi_power_profile.hpp>

namespace cx::os::kernel::acpi
{
    struct AcpiFadt : AcpiSdtBase
    {
        Acpi32BitPointer<AcpiFacs> firmware_control; // FACS
        Acpi32BitPointer<AcpiDsdt> dsdt;
        
        uint8_t acpi1_0_reserved;
        
        AcpiPowerProfile preferred_power_profile;
        
        uint16_t sci_interrupt;
        uint32_t smi_command_port;
        
        bool acpi_enable;
        bool acpi_disable;
        
        /* TODO */
        
        static constexpr auto AcpiSdtSignature = "FACP";
    };
}

#endif /* acpi_fadt_h */
