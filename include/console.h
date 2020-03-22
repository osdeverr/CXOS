//
//  console.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef console_h
#define console_h
#include <stdint.h>

namespace Console
{
    void Initialize();
    void Clear();
    
    void Write(const uint8_t c);
    void Write(const char* str);
}

#endif /* console_h */
