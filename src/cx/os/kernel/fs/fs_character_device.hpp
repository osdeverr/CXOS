//
//  fs_character_device.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 10.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_character_device_h
#define fs_character_device_h

#include <cx/os/kernel/fs/fs_node_impl.hpp>
#include <cx/os/kernel/fs/fs_character_stream.hpp>

namespace cx::os::kernel::fs
{
    class FsCharacterDevice : public FsNodeImpl<FsCharacterDevice>
    {
    public:
        FsCharacterDevice(const FsNodeName& name, bool exclusive)
        : FsNodeImpl<FsCharacterDevice>(name), _exclusive(exclusive)
        {}
        
        std::shared_ptr<IFsCharacterStream> OpenCharacterStream()
        {
            return OpenDeviceImpl();
        }
        
    protected:
        virtual std::shared_ptr<IFsCharacterStream> OpenDeviceImpl() = 0;
        
    private:
        bool _exclusive = false;
        
    public:
        static constexpr auto FsNodeImpl_NodeType = FsNodeType::CharacterDevice;
    };
}

#endif /* fs_character_device_h */
