//
//  rtl8139.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/devices/net/rtl8139/rtl8139.hpp>
#include <cx/os/kernel/devices/pci/pci.hpp>
#include <cx/os/kernel/kprintf.hpp>
#include <cx/os/kernel/interrupts/interrupts.hpp>

namespace cx::os::kernel::net::rtl8139::detail
{
    constexpr pci::PciVendorId kRtlPciVendor = 0x10EC;
    constexpr pci::PciDeviceId kRtlPciDevice = 0x8139;
    
    bool gInitialized = false;
    
    ports::PortId gIoBase = 0;
    MacAddress gMacAddress = {0};
    
    uint8_t gReceiveBuf[16384] = {0};
    
    
}

void cx::os::kernel::net::rtl8139::TryInitialize()
{
    using namespace detail;
    using namespace pci;
    
    
    
    for(PciBus bus = 0; bus < 256; bus++)
    {
        for(PciSlot slot = 0; slot < 32; slot++)
        {
            auto vendor = GetPciDeviceVendor(bus, slot);
            auto device = GetPciDeviceId(bus, slot);
            
            if(vendor == kRtlPciVendor && device == kRtlPciDevice)
            {
                auto bar0 = GetPciDeviceBar(bus, slot, 0, 0);
                
                kprintf("\e[92mFound an RTL8139 card @ %02u:%02u.0: proceeding with setup!\n", (int) bus, (int) slot);
                gIoBase = bar0.addr;
                
                // Powering on...
                ports::WriteB(gIoBase + kRTL8139Port_Config1, 0);
                
                for(auto i = 0; i < 8; i++)
                    gMacAddress[i] = ports::ReadB(gIoBase + kRTL8139Port_MacAddrBase + i);
                
                kprintf("    Card MAC address: {%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X}\n",
                        gMacAddress[0], gMacAddress[1], gMacAddress[2], gMacAddress[3],
                        gMacAddress[4], gMacAddress[5], gMacAddress[6], gMacAddress[7]
                        );
                
                auto irq_line = GetPciDeviceIrqLine(bus, slot);
                kprintf("    Card interrupt line: %d\n", irq_line);
                kprintf("    Card I/O Base: 0x%04X\n", gIoBase);
                
                kprintf("    --- Beginning card reset ---\n");
                
                ports::WriteB(gIoBase + kRTL8139Port_Command, kRTL8139Command_SoftwareReset);
                while((ports::ReadB(gIoBase + kRTL8139Port_Command) & kRTL8139Command_SoftwareReset))
                {}
                
                kprintf("    --- Card reset done ---\n");
                
                // Tell our card the rx buffer address
                ports::WriteL(gIoBase + kRTL8139Port_ReceiveBufferStart, (uint32_t) gReceiveBuf);
                
                // Setup ISRs
                ports::WriteW(gIoBase + kRTL8139Port_InterruptSetup, 0x0005);
                
                // Enable RX/TX!
                ports::WriteB(gIoBase + kRTL8139Port_Command, kRTL8139Command_EnablePacketExchange);
                
                interrupts::AddIrqHandler(
                                          irq_line,
                                          [](const interrupts::InterruptRegisterState& regs)
                                          {
                                              kprintf(" *** RTL8139: Got an RX interrupt!\n");
                                          }
                                          );
                
                kprintf("\e[92mRTL8139 @ %02u:%02u.0: setup finished; ready to receive\n", (int) bus, (int) slot);
                
                gInitialized = true;
                return;
            }
        }
    }
    
    kprintf("\e[91mCould not find an RTL8139 card. Networking will not be available.\n");
}

const cx::os::kernel::net::MacAddress& cx::os::kernel::net::rtl8139::GetMacAddress()
{
    return detail::gMacAddress;
}


