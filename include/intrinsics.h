//
//  intrinsics.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef intrinsics_h
#define intrinsics_h

#define CX_OS_IRQS_ON() asm("sti")
#define CX_OS_IRQS_OFF() asm("cli")
#define CX_OS_HALT_CPU() asm("hlt")

#endif /* intrinsics_h */
