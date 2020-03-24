//
//  node.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/24/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef node_h
#define node_h
#include <stdint.h>
#include <tlist.h>

namespace FS
{
    enum eNodeType
    {
        eNodeType_Directory,
        eNodeType_File,
        eNodeType_Link
    };
    
    class Node : public tList<Node>
    {
    public:
        Node* Parent() { return mpParent; }
        const Node* Parent() const { return mpParent; }
        const char* Name() const { return mName; }
        const eNodeType Type() const { return mType; }
        void SetParent(Node* parent) { mpParent = parent; }
        
        template<typename T>
        T* As() { return (T*) this; }
    protected:
        virtual ~Node() {}
        Node(eNodeType type, const char* name) : mType(type), mName(name), mpParent(nullptr) {}
        
        Node* mpParent;
        eNodeType mType;
        const char* mName;
    };
}

#endif /* node_h */
