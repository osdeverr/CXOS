//
//  assert.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef assert_h
#define assert_h
#include <panic.h>

#define ASSERT_STRINGIFY(x) # x
#define assert(expression) (expression) ? (void) 0 : Panic::Halt("Kernel Assertion Failed: " ASSERT_STRINGIFY(expression))

#endif /* assert_h */
