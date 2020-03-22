//
//  kprint.c
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/21/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <kprint.h>
void kprint(const char* text)
{
    static int pos = 0;
    while(*text)
        *(volatile char*) (0xB8000 + pos++ * 2) = *text++;
}
