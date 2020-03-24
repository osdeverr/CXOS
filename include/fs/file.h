//
//  file.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/24/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef file_h
#define file_h
#include <fs/node.h>

namespace FS
{
    class File : public Node
    {
    public:
        virtual ~File() {}
        File(const char* name) : Node(eNodeType_File, name) {}
    };
}

#endif /* file_h */
