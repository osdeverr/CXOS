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
 
namespace cx::os::kernel::detail
{
    vga::VgaConsole gVgaConsole;
}

void cx::os::kernel::BeginKernelStartup(const multiboot_info_t& boot_info)
{
    using namespace detail;
    gVgaConsole.UpdateCursor(0, 0);
    gVgaConsole.ClearScreen();
    
    console::SetupAnsiConsole(&gVgaConsole);
    
    memory::AddMemoryRegion(nullptr, 0x1234);
    memory::AddMemoryRegion((void*) 0x50000, 0x50289);
    
    memory::DumpMemoryRegions();
    
    auto ptr = malloc(0x1400);
    printf("Allocated 0x1400 @ 0x%08X\n", ptr);
    
    memory::DumpMemoryRegions();
    
    free(ptr);
    printf("Memory freed\n");
    
    memory::DumpMemoryRegions();
    
    printf("hi 0x%08X\n", 1024);
    
    struct Foo : MinListNode<Foo>
    {
        int x;
    };
    
    Foo a, b, c, d;
    a.x = 5;
    b.x = 6;
    c.x = 10;
    d.x = 30;
    
    MinList<Foo> list;
    list.InsertNode(&a);
    list.InsertNode(&b);
    list.InsertNode(&c);
    list.InsertNode(&d);
    
    for(auto& value : list)
        printf("value=%d\n", value.x);
}
