//
//  fs_directory.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_directory_h
#define fs_directory_h
#include <cx/os/kernel/fs/fs_node_impl.hpp>
#include <cx/stl/list.hpp>
#include <cx/stl/shared_ptr.hpp>

namespace cx::os::kernel::fs
{
    using FsNodeList = std::list<std::shared_ptr<FsNode>>;
    
    class FsDirectory : public FsNodeImpl<FsDirectory>
    {
    public:
        FsDirectory(const FsNodeName& name, FsNode* parent = nullptr)
        : FsNodeImpl<FsDirectory>(name, parent)
        {}
        
        void AddDirectoryEntry(std::shared_ptr<FsNode> node)
        {
            node->ConnectParent(this);
            _nodes.push_back(node);
        }
        
        void RemoveDirectoryEntry(std::shared_ptr<FsNode> node)
        {
            // TODO no list remove yet
        }
        
        std::shared_ptr<FsNode> FindDirectoryEntry(FsNodeName name)
        {
            for(auto& node : _nodes)
                if(node->GetName() == name)
                    return node;
            
            return nullptr;
        }
        
        const FsNodeList& GetDirectoryEntries() const
        {
            return _nodes;
        }
        
    private:
        FsNodeList _nodes;
        
    public:
        static constexpr auto FsNodeImpl_NodeType = FsNodeType::Directory;
    };
}


#endif /* fs_directory_h */
