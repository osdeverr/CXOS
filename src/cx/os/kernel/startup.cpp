//
//  startup.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <cx/os/kernel/startup.hpp>
#include <cx/os/kernel/devices/console/console.hpp>
#include <cx/os/kernel/devices/vga/vga_console.hpp>
#include <cx/util/min_list.hpp>
#include <cx/os/kernel/memory/memory.hpp>
#include <cx/os/kernel/io/ports.hpp>
#include <cx/os/kernel/debug/stack_trace.hpp>
#include <cx/util/fixed_vector.hpp>
#include <cx/os/kernel/interrupts/interrupts.hpp>
#include <cx/os/kernel/kprintf.hpp>
#include <cx/os/kernel/intrinsics.h>

#include <cx/stl/shared_ptr.hpp>
#include <cx/stl/function.hpp>

#include <cx/os/kernel/devices/timers/timer.hpp>
#include <cx/os/kernel/devices/timers/pit_timer.hpp>

#include <cx/os/kernel/devices/ps2/ps2_protocol.hpp>
#include <cx/os/kernel/devices/ps2/ps2_keyboard.hpp>

#include <printf.h>


extern char __cx_kernel_start_marker, __cx_kernel_end_marker;
 
namespace cx::os::kernel::detail
{
    vga::VgaConsole gVgaConsole;
    std::shared_ptr<timers::Timer> gTimer;
    
    const auto gKernelStart = &__cx_kernel_start_marker;
    const auto gKernelEnd = &__cx_kernel_end_marker;
}

void cx::os::kernel::BeginKernelStartup(const multiboot_info_t& boot_info)
{
    using namespace detail; 
    gVgaConsole.UpdateCursor(0, 0);
    gVgaConsole.ClearScreen();
    
    console::SetupAnsiConsole(&gVgaConsole);
    gVgaConsole.ToggleVgaCursor(true, 0, 15);
    
    kprintf("Loaded CXOS kernel @ 0x%08X..0x%08X\n", gKernelStart, gKernelEnd);
        
    kprintf("Setting up memory maps\n");
    auto entry = (mmap_entry_t*) boot_info.mmap_addr;
    
    while((unsigned int) entry < boot_info.mmap_addr + boot_info.mmap_length) {
        // We ignore the 0 entry since that causes issues
        if(entry->type == 1 && entry->base_addr_low != 0)
        {
            auto addr = (char*) entry->base_addr_low;
            auto length = entry->length_low;
            
            if(addr >= gKernelStart && addr < gKernelEnd)
            {
                kprintf("Found in-kernel MM bit - adjusting\n");
                
                addr = gKernelEnd;
                length -= (addr - gKernelStart);
            }
            
            kprintf("Adding memory region type=%d 0x%08X..0x%08X\n", entry->type, addr, (uint32_t) addr + length);
            memory::AddMemoryRegion(addr, length);
        }
        entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    }
    
    memory::DumpMemoryRegions();
    
    kprintf("Setting up interrupts\n");
    interrupts::SetupInterruptTable();
    
    kprintf("Setting up timers\n");
    gTimer = std::make_shared<timers::PitTimer>(1000);
    
    printf("Welcome to \e[94mCXOS\e[0m\n");
    
    /*
        Keyboard stuff
     
        interrupts::AddIrqHandler(interrupts::IrqType::PS2Keyboard,
                                  [](const interrupts::InterruptRegisterState& regs)
                                  {
                                      auto raw_sc = ports::ReadB(ps2::kPs2ControllerPort_Data);
                                      auto sc = raw_sc &~ ps2::kPs2KeyboardScancodeFlags_KeyReleased;
                                      auto key = ps2::ResolveKeyScancode(sc);
                                      auto released = raw_sc & ps2::kPs2KeyboardScancodeFlags_KeyReleased;
     
                                      kprintf("Key %s => (sc=%d, key='%c')\n", (released) ? "released" : "pressed", sc, key);
                                  }
                                  );
     */
    
    while(true)
    {
        CX_OS_IRQS_ON();
        CX_OS_HALT_CPU();
    }
}
