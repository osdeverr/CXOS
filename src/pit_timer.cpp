//
//  pit_timer.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 04.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <interrupts.hpp>
#include <pit_timer.hpp>

#include <kprintf.hpp>

namespace cx::os::kernel::timers::detail
{
    constexpr auto kPitTimerFreqBase = 1193182;
}

cx::os::kernel::timers::PitTimer::PitTimer(TimerFrequency frequency)
{
    kprintf("cx::os::kernel::timers::PitTimer created\n");
    
    static int i = 0;
    interrupts::AddIrqHandler(interrupts::IrqType::PIT,
                              [this](const interrupts::InterruptRegisterState& regs)
                              {
                                  for(auto& cb : _callbacks)
                                      cb();
                              }
                              );
    
    SetFrequencyImpl(frequency);
}

void cx::os::kernel::timers::PitTimer::SetFrequencyImpl(TimerFrequency frequency)
{
    // TODO: Set command stuff
    ports::WriteB(kPitTimerPort_Command, 0x36);
    
    int divisor = 1193180 / frequency;
    ports::WriteB(kPitTimerPort_Channel0, divisor & 0xFF);
    ports::WriteB(kPitTimerPort_Channel0, divisor >> 8);
}

cx::os::kernel::timers::PitTimer::~PitTimer()
{
    kprintf("cx::os::kernel::timers::PitTimer destroyed; create a new timer?\n");
}
