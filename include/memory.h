//
//  memory.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef memory_h
#define memory_h

extern "C" void* malloc(int) { return nullptr; }
extern "C" void free(void*) { }

#endif /* memory_h */
