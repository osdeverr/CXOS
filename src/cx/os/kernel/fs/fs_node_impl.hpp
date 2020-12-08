//
//  fs_node_impl.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_node_impl_h
#define fs_node_impl_h
#include <cx/os/kernel/fs/fs_node.hpp>

namespace cx::os::kernel::fs
{
    template<class T>
    class FsNodeImpl : public FsNode
    {
    public:
        FsNodeImpl<T>(const FsNodeName& name, FsNode* parent = nullptr)
        : FsNode(T::FsNodeImpl_NodeType, name, parent)
        {}
        
    protected:
        using FsNode::_type;
        using FsNode::_name;
        using FsNode::_parent;
    };
}

#endif /* fs_node_impl_h */
