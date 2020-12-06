//
//  pci.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/devices/pci/pci.hpp>
#include <cx/os/kernel/io/ports.hpp>

namespace cx::os::kernel::pci::detail
{
    uint16_t PciReadConfigWord(PciBus bus, PciSlot slot, PciFunction function, uint8_t offset)
    {
        uint32_t addr;
        uint32_t bus32 = bus;
        uint32_t slot32 = slot;
        uint32_t func32 = function;
        addr = (uint32_t)((bus32 << 16) | (slot32 << 11) |
                          (func32 << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
        
        ports::WriteL(kPciControllerPort_CfgAddress, addr);
        return (uint16_t)((ports::ReadL(kPciControllerPort_CfgData) >> ((offset & 2) * 8)) & 0xffff);
    }
}

cx::os::kernel::pci::PciVendorId cx::os::kernel::pci::GetPciDeviceVendor(PciBus bus, PciSlot slot, PciFunction function)
{
    return detail::PciReadConfigWord(bus, slot, function, kPciHeaderOffset_VendorId);
}

cx::os::kernel::pci::PciDeviceId cx::os::kernel::pci::GetPciDeviceId(PciBus bus, PciSlot slot, PciFunction function)
{
    return detail::PciReadConfigWord(bus, slot, function, kPciHeaderOffset_DeviceId);
}

cx::os::kernel::pci::PciDevType cx::os::kernel::pci::GetPciDeviceType(PciBus bus, PciSlot slot, PciFunction function)
{
    auto combined = detail::PciReadConfigWord(bus, slot, function, kPciHeaderOffset_ClassSubClass);
    
    auto dev_class = (PciClass) (combined >> 8);
    auto dev_subclass = (PciSubClass) (combined & 0xFF);
    
    return {dev_class, dev_subclass};
}

cx::os::kernel::pci::PciHdrType cx::os::kernel::pci::GetPciDeviceHdrType(PciBus bus, PciSlot slot, PciFunction function)
{
    uint8_t byte = detail::PciReadConfigWord(bus, slot, function, kPciHeaderOffset_HeaderType);
    return (PciHdrType) byte;
}

cx::os::kernel::pci::PciBar cx::os::kernel::pci::GetPciDeviceBar(PciBus bus, PciSlot slot, PciFunction function, size_t bar_id)
{
    auto hdr_type = GetPciDeviceHdrType(bus, slot, function);
    if(hdr_type &~ kPciHdrType_MultipleFuncs != kPciHdrType_Regular)
        return {};
    
    uint8_t off = bar_id * 2;
    uint16_t bar_low  = detail::PciReadConfigWord(bus, slot, function, kPciHeaderOffset_BarAddrBase + off);
    uint16_t bar_high = detail::PciReadConfigWord(bus, slot, function, kPciHeaderOffset_BarAddrBase + off + 1);
    if((bar_low & 1) == 0)
    {
        if((bar_low & ~0b110) == 0x00) //32-bit bar
        {
            uint32_t ret = (uint32_t) bar_low | (uint32_t) (bar_high << 16);
            ret &= ~0b1111;
            return {PciBarType::MemoryMapped, ret};
        }
    }
    else
    {
        uint32_t ret = (uint32_t) bar_low | (uint32_t) (bar_high << 16);
        ret &= ~0b11;
        return {PciBarType::PortIO, ret};
    }
    
    return {};
}
