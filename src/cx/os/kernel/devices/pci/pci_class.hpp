//
//  pci_class.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pci_class_h
#define pci_class_h
#include <stdint.h>

namespace cx::os::kernel::pci
{
    enum class PciClass
    {
        Unclassified = 0x0,
        MassStorageController,
        NetworkController,
        DisplayController,
        MultimediaController,
        MemoryController,
        BridgeDevice,
        SimpleCommunicationController,
        BaseSystemPeripheral,
        InputDeviceController,
        DockingStation,
        Processor,
        SerialBusController,
        WirelessController,
        IntelligentController,
        SatelliteCommunicationController,
        EncryptionController,
        SignalProcessingController,
        ProcessingAccelerator,
        NonEssentialInstrumentation,
        
        VendorSpecific = 0xFF
    };
}

#endif /* pci_class_h */
