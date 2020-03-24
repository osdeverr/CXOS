//
//  root.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/24/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef root_h
#define root_h
#include <fs/node.h>

namespace FS
{
    void Initialize();
    extern Node* gRoot;
}

#endif /* root_h */
