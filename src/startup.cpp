//
//  startup.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <startup.hpp>
#include <console.hpp>
#include <vga_console.hpp>
#include <printf.h>
#include <min_list.hpp>
#include <memory.hpp>
#include <ports.hpp>
#include <stack_trace.hpp>
#include <fixed_vector.hpp>
#include <interrupts.hpp>
#include <kprintf.hpp>
#include <intrinsics.h>

#include <stl/shared_ptr.hpp>
#include <stl/function.hpp>

extern char __cx_kernel_start_marker, __cx_kernel_end_marker;
 
namespace cx::os::kernel::detail
{
    vga::VgaConsole gVgaConsole;
    
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
    
    kprintf("Setting up interrupts\n");
    interrupts::SetupInterruptTable();
     
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
    
    printf("Welcome to \e[94mCXOS\e[0m\n");
    
    interrupts::AddIrqHandler(interrupts::IrqType::PIT,
                              [](const interrupts::InterruptRegisterState& regs)
                              {
                                  kprintf("Timer hit!\n");
                              }
                              );
    
    interrupts::AddIrqHandler(interrupts::IrqType::PS2Keyboard,
                              [](const interrupts::InterruptRegisterState& regs)
                              {
                                  kprintf("I hit the KEY!\n");
                              }
                              );
    
    printf("\n");
    printf("\e[93mish1.0\e[90m # \e[0m\n\n");
    
    auto p1 = std::make_shared<int>(4);
    auto p2 = p1;
   
    int fCounter = 0;
    std::function<void()> cprint =
    [&fCounter]()
    {
        printf("fCounter=%d\n", fCounter++);
    };
    
    for(auto i = 0; i < 5; i++)
        cprint();
    
    printf("p1=%d; p2=%d\n", *p1, *p2);
    
    // while(1) asm("sti; hlt");
}
