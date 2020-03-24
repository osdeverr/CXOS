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
void  operator delete(void* ptr, unsigned long size) { free(ptr); }
void* operator new[](unsigned long size) { return malloc(size); }
void  operator delete[](void* ptr) { free(ptr); }

void *__dso_handle;

extern "C"
{
int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso) { return 1; }
void __cxa_finalize(void *f) {}
}
