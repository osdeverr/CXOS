//
//  kprintf.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef kprintf_h
#define kprintf_h
#include <printf.h>

template<class... Ts>
inline void kprintf(const char* fmt, Ts... args)
{
    printf("\e[90m *** \e[39m");
    printf(fmt, args...);
    printf("\e[0m");
}

template<class... Ts>
inline void klog(const char* category, const char* fmt, Ts... args)
{
    printf("\e[90m *** \e[93m%s\e[90m: ", category);
    printf(fmt, args...);
    printf("\e[0m");
}

#endif /* kprintf_h */
