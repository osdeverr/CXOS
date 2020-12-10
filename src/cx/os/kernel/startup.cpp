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

#include <cx/os/kernel/devices/acpi/acpi_rsdp_v1.hpp>
#include <cx/os/kernel/devices/acpi/acpi_fadt.hpp>

#include <cx/os/kernel/devices/acpi/aml/stream_reader.hpp>
#include <cx/os/kernel/devices/acpi/aml/scope_def.hpp>
#include <cx/os/kernel/devices/acpi/aml/pkg_lead_byte.hpp>
#include <cx/os/kernel/devices/acpi/aml/aml_opcode.hpp>

#include <cx/os/kernel/devices/pci/pci.hpp>
#include <cx/os/kernel/devices/net/rtl8139/rtl8139.hpp>

#include <cx/stl/list.hpp>

#include <cx/os/kernel/fs/fs_directory.hpp>
#include <cx/os/kernel/fs/fs_file.hpp>
#include <cx/os/kernel/fs/fs.hpp>

#include <printf.h>
#include <string.h>

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
    CX_OS_IRQS_ON();
    
    kprintf("Setting up timers\n");
    gTimer = std::make_shared<timers::PitTimer>(1000);
    
    kprintf("Setting up PCI\n");
    {
        using namespace pci;
        for(PciBus bus = 0; bus < 256; bus++)
        {
            for(PciSlot slot = 0; slot < 32; slot++)
            {
                auto vendor = GetPciDeviceVendor(bus, slot, 0);
                auto device = GetPciDeviceId(bus, slot, 0);
                
                if(vendor != kInvalidVendorId)
                {
                    auto type = GetPciDeviceType(bus, slot, 0);
                    auto hdr = GetPciDeviceHdrType(bus, slot, 0);
                    auto bar0 = GetPciDeviceBar(bus, slot, 0, 0);
                    
                    kprintf("PCI @ %02u:%02u.0 => 0x%04X-0x%04X\n", (int) bus, (int) slot, (int) vendor, (int) device);
                    kprintf("      class=0x%02X subclass=0x%02X\n", (int) type.dev_class, (int) type.dev_subclass);
                    kprintf("      hdr_type=0x%02X\n", (int) hdr);
                    kprintf("      bar0=(type=%d; addr=0x%08X)\n", (int) bar0.type, (uint16_t) bar0.addr);
                }
            }
        }
    }
    
    kprintf("Setting up networking\n");
    net::rtl8139::TryInitialize();
    
    kprintf("Setting up ACPI\n");
    {
        using namespace acpi;
        
        char* ptr = nullptr;
        
        while(strncmp(ptr, AcpiRsdpValidSignature, sizeof(AcpiRsdpValidSignature)))
            ptr += 16;
        
        auto rsdp = reinterpret_cast<AcpiRsdpV1*>(ptr);
        auto rsdt = rsdp->rsdt;
        
        /*
        printf("\n");
        kprintf("\e[92mRSDP\e[0m: Found @ \e[93m0x%08X\e[0m:\n", rsdp);
        kprintf("      Valid=%d\n", rsdp->IsValid());
        kprintf("      ACPI Revision=%d\n", rsdp->acpi_revision);
        kprintf("      RSDT Pointer=0x%08X\n", rsdt);
        kprintf("      RSDT Valid=%d\n", rsdt->IsValid());
        kprintf("      RSDT Signature='%c%c%c%c'\n", rsdt->signature[0], rsdt->signature[1], rsdt->signature[2], rsdt->signature[3]);
        
        int i = 1;
        for(auto& table : rsdt->GetTables())
            kprintf("      Table='%c%c%c%c' (l=%d)\n", table->signature[0], table->signature[1], table->signature[2], table->signature[3], table->length);
         */
        
        auto fadt = rsdt->FindTable<AcpiFadt>();
        if(fadt)
        {
            kprintf("      FADT Valid=%d\n", fadt->IsValid());
            kprintf("      Preferred Power Profile=%d\n", fadt->preferred_power_profile);
            kprintf("      SCI Interrupt=%d\n", fadt->sci_interrupt);
            kprintf("      SMI Command Port=0x%02X\n", fadt->smi_command_port);
            kprintf("      ACPI On  Byte=0x%02X\n", fadt->acpi_enable);
            kprintf("      ACPI Off Byte=0x%02X\n", fadt->acpi_disable);
            
            ports::WriteB(fadt->smi_command_port, fadt->acpi_enable);
            
            auto dsdt = fadt->dsdt;
            kprintf("      DSDT=0x%08X (valid=%d, len=%d, oem=0x%08X)\n", dsdt, dsdt->IsValid(), dsdt->length, dsdt->oem_id);
            
            /*
            auto dsdt_length = (dsdt->length - sizeof(AcpiSdtBase));
            auto reader = aml::StreamReader(dsdt->aml_code, dsdt->aml_code + dsdt_length);
            
            while(!reader.IsEof())
            {
                using namespace aml;
                auto opcode = reader.Read<AmlOpcode>();
                if(opcode == AmlOpcode::ScopeOp)
                {
                    kprintf("Caught ScopeOp\n");
                    
                    auto pkg = reader.Read<PkgLeadByte>();
                    uint32_t length = 0;
                    uint8_t b_offset = 4;
                    
                    if(pkg.byte_length)
                        length = pkg.size_if_0byte;
                    else
                        length |= pkg.lsb_nibble;
                    
                    for(auto i = 0; i < pkg.byte_length; i++, b_offset += 8)
                        length |= (reader.Read<uint8_t>() << b_offset);
                    
                    kprintf("     PkgLeadByte: (l=%d bl=%d) sizeof=%d\n", length, pkg.byte_length, sizeof pkg);
                     
                    char name[5] = {0};
                    for(auto i = 0; i < 4; i++)
                    {
                        auto c = reader.Read<char>();
                        name[i] = c;
                    }
                    
                    kprintf("     Object Length: %d\n", length);
                    kprintf("     Object Name: %s\n", name);
                    
                    auto the_opcode = reader.Read<AmlOpcode>();
                    kprintf("     NEXT opcode: 0x%02X\n", the_opcode);
                    
                    break;
                }
            }
             */
        }
        else
        {
            kprintf("\e[91m ! FADT table NOT found.\e[0m");
        }
    }
    
    kprintf("Setting up keyboards\n");
    ps2::InitializePs2Keyboard();
    
    kprintf("Welcome to \e[94mCXOS\e[0m\n");
    printf("\n");
    
    std::list<int> ints;
    ints.push_back(4);
    ints.push_back(5);
    ints.push_back(6);
    ints.push_back(10);
    ints.push_front(14);
    
    for(auto& i : ints)
        kprintf("%d\n", i);
    
    auto six = ints.find(6);
    kprintf("six=%d\n", *six);
    
    {
        using namespace fs;
        
        auto& root = GetFilesystemRoot();
        
        auto bin = std::make_shared<FsDirectory>("bin");
        auto usr = std::make_shared<FsDirectory>("usr");
        auto home = std::make_shared<FsDirectory>("home");
        bin->AddDirectoryEntry(std::make_shared<FsFile>("ish", nullptr, 25591));
        usr->AddDirectoryEntry(std::make_shared<FsDirectory>("share"));
        home->AddDirectoryEntry(std::make_shared<FsDirectory>("cxos-install"));
        
        root.AddDirectoryEntry(bin);
        root.AddDirectoryEntry(usr);
        root.AddDirectoryEntry(home);
        
        char passwd_data[] = "osdever=12345\ntest=123test";
        auto passwd = std::make_shared<FsFile>(".passwd", passwd_data, sizeof passwd_data);
        root.AddDirectoryEntry(passwd);
        
        std::function<void(const FsNode&, int)> printout;
        printout =
        [&printout](const FsNode& node, int tabs)
        {
            auto tabulate =
            [tabs](int extra = 0)
            {
                for(auto i = 0; i < tabs + extra; i++)
                    printf("    ");
            };
            
            tabulate();
            auto name = node.GetName().AsCharPtr();
            printf("/%s - ", name);
            
            auto type = node.GetType();
            
            switch(type)
            {
                case FsNodeType::Directory:
                {
                    printf("\e[32mFsDirectory\e[0m\n", type);
                    
                    auto dir = node.As<FsDirectory>();
                    for(auto& entry : dir->GetDirectoryEntries())
                    {
                        printout(*entry, tabs + 1);
                    }
                    break;
                } 
                case FsNodeType::RegularFile:
                {
                    printf("\e[32mFsFile\e[0m\n", type);
                    
                    auto file = node.As<FsFile>();
                    tabulate(1);
                    printf("Size: %u bytes\n", file->GetFileSize());
                    break;
                }
                default:
                    printf("\e[33mUnknown node type {%d}\e[0m\n", type);
                    break;
            };
        };
        
        printout(root, 1);
        
        if(auto stream = passwd->OpenFileStream())
        {
            int c = 0;
            while((c = stream->ReadByte()) != -1)
                printf("%c", (char) c);
            printf("[EOF]\n");
            stream->Close();
        }
        else
        {
            kprintf("\e[91m ! File could NOT open.\e[0m");
        }
    }
    
    printf("ish1.0# ");
    
    char c = 0;
    while(c != '\n')
    {
        c = ps2::Ps2KeyboardGetChar();
        printf("%c", c);
    }
}
