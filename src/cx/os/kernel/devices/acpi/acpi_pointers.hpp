//
//  acpi_pointers.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef acpi_pointers_h
#define acpi_pointers_h
#include <cx/stl/nullptr.hpp>
#include <stdint.h>

namespace cx::os::kernel::acpi
{
    template<class BaseT, class PtrT>
    class AcpiPointerConvertibleBase
    {
    public:
        AcpiPointerConvertibleBase(std::nullptr_t)
        : _ptr{0}
        {}
        
        AcpiPointerConvertibleBase(BaseT base)
        : _ptr(base)
        {}
        
        AcpiPointerConvertibleBase(PtrT* ptr)
        : _ptr(static_cast<BaseT>(ptr))
        {}
        
        operator BaseT() const
        {
            return _ptr;
        }
        
        operator PtrT*() const
        {
            return (PtrT*) _ptr;
        }
        
        PtrT& operator*()
        {
            return *(PtrT*) *this;
        }
        
        PtrT* operator->()
        {
            return (PtrT*) *this;
        }
        
    private:
        BaseT _ptr;
    };
    
    template<class T>
    using Acpi32BitPointer = AcpiPointerConvertibleBase<uint32_t, T>;
    
    template<class T>
    using Acpi64BitPointer = AcpiPointerConvertibleBase<uint64_t, T>;
}

#endif /* acpi_pointers_h */
