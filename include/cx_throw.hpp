//
//  cx_throw.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef cx_throw_h
#define cx_throw_h

#ifdef CX_KERNEL
#include <panic.hpp>
#define CX_THROW_FATAL(message) cx::os::kernel::debug::TriggerKernelPanic(message)
#else
#include <stdexcept>
#define CX_THROW_FATAL(message) throw std::runtime_error(message)
#endif

#endif /* cx_throw_h */
