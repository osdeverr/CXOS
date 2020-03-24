//
//  directory.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/24/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef directory_h
#define directory_h
#include <fs/node.h>
#include <tlist.h>

namespace FS {
    class Directory : public Node
    {
    public:
        Directory(const char* name) : Node(eNodeType_Directory, name) {}
        tList<Node>& GetChildren() { return mChildren; }
        void Add(Node* child) { mChildren.push_back(child); child->SetParent(this); }
        
        void Debug_Printout(int tabs = 0);
    private:
        list<Node> mChildren;
    };
}

#endif /* directory_h */
