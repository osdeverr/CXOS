//
//  aml_opcode.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef aml_opcode_h
#define aml_opcode_h
#include <stdint.h>

namespace cx::os::kernel::acpi::aml
{
    enum class AmlOpcode : uint8_t
    {
        ScopeOp = 0x20
    };
}

#endif /* aml_opcode_h */
