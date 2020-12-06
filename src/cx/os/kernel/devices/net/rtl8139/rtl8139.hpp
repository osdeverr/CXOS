//
//  rtl8139.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef rtl8139_h
#define rtl8139_h
#include <cx/os/kernel/devices/net/mac_address.hpp>
#include <cx/os/kernel/io/ports.hpp>

namespace cx::os::kernel::net::rtl8139
{
    enum RTL8139Port : ports::PortId
    {
        kRTL8139Port_MacAddrBase = 0x0,
        kRTL8139Port_MarAddrBase = 0x8,
        
        kRTL8139Port_ReceiveBufferStart = 0x30,
        
        kRTL8139Port_Command = 0x37,
        kRTL8139Port_InterruptSetup = 0x3C,
        
        kRTL8139Port_PacketAcceptConfig = 0x44,
        
        kRTL8139Port_Config1 = 0x52,
    };
        
    enum RTL8139Command : uint8_t
    {
        kRTL8139Command_SoftwareReset = 0x10,
        kRTL8139Command_EnablePacketExchange = 0x0C,
    };
    
    void TryInitialize();
    const MacAddress& GetMacAddress();
}

#endif /* rtl8139_h */
