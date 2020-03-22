//
//  cxxnew.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <memory.h>

void* operator new(unsigned long size) { return malloc(size); }
void  operator delete(void* ptr) { free(ptr); }
void* operator new[](unsigned long size) { return malloc(size); }
void  operator delete[](void* ptr) { free(ptr); }
