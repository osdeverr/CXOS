//
//  fs_node.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_node_h
#define fs_node_h
#include <cx/os/kernel/fs/fs_node_type.hpp>
#include <cx/os/kernel/fs/fs_character_stream.hpp>
#include <cx/os/kernel/fs/fs_block_stream.hpp>

#include <cx/stl/shared_ptr.hpp>

#include <string.h>

namespace cx::os::kernel::fs
{
    // TODO: change this
    class FsNodeName
    {
    public:
        FsNodeName(const char* name)
        {
            strcpy(_name, name);
        }
        
        const char* AsCharPtr() const
        {
            return _name;
        }
        
    private:
        char _name[256] = "";
    };
    
    class FsNode
    {
    public:
        virtual ~FsNode() = default;
        
    protected:
        FsNode(FsNodeType type, const FsNodeName& name, FsNode* parent = nullptr)
        : _type(type), _name(name), _parent(parent)
        {}
        
    public:
        template<class T>
        T* As()
        {
            if(_type == T::FsNodeImpl_NodeType)
                return (T*) this;
            else
                return nullptr;
        }
        
        template<class T>
        const T* As() const
        {
            if(_type == T::FsNodeImpl_NodeType)
                return (const T*) this;
            else
                return nullptr;
        }
        
        FsNodeType GetType() const
        {
            return _type;
        }
        
        const FsNodeName& GetName() const
        {
            return _name;
        }
        
        void Rename(const FsNodeName& name)
        {
            _name = name;
        }
        
        FsNode* GetParent() const
        {
            return _parent;
        }
        
        void ConnectParent(FsNode* parent)
        {
            _parent = parent;
        }
        
        virtual std::shared_ptr<IFsCharacterStream> OpenCharacterStream()
        {
            return nullptr;
        }
        
        virtual std::shared_ptr<IFsBlockStream> OpenBlockStream()
        {
            return nullptr;
        }
        
    protected:
        FsNodeType _type;
        FsNodeName _name;
        FsNode* _parent;
    };
}

#endif /* fs_node_h */
