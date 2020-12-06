//
//  acpi_root_sdt_base.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_root_sdt_base_h
#define acpi_root_sdt_base_h
#include <cx/os/kernel/devices/acpi/acpi_sdt_base.hpp>
#include <cx/os/kernel/devices/acpi/acpi_pointers.hpp>
#include <string.h>

namespace cx::os::kernel::acpi
{
    template<class RootSdt>
    class AcpiSdtList
    {
    public:
        using pointer_type = typename RootSdt::pointer_type;
        
        AcpiSdtList(const RootSdt* rsdt)
        : _rsdt(rsdt)
        {}
        
        const pointer_type* begin() const
        {
            return &_rsdt->tables[0];
        }
        
        const pointer_type* end() const
        {
            return &_rsdt->tables[(_rsdt->length - sizeof(AcpiSdtBase)) / sizeof(pointer_type)];
        }
        
    private:
        const RootSdt* _rsdt;
    };
    
    template<template<class> class Pointer>
    struct AcpiRootSdtBase : AcpiSdtBase
    {
        using pointer_type = Pointer<AcpiSdtBase>;
        
        Pointer<AcpiSdtBase> tables[1];
        
        AcpiSdtList<AcpiRootSdtBase<Pointer>> GetTables() const
        {
            return {this};
        }
        
        template<class T>
        T* FindTable() const
        {
            auto iterable = GetTables();
            for(auto& table : iterable)
                if(!strncmp(table->signature, T::AcpiSdtSignature, 4))
                    return reinterpret_cast<T*>(static_cast<AcpiSdtBase*>(table));
            
            return nullptr;
        }
    };
}

#endif /* acpi_root_sdt_base_h */
