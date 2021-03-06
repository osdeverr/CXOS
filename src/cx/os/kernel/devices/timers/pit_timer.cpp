//
//  pit_timer.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 04.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <cx/os/kernel/interrupts/interrupts.hpp>
#include <cx/os/kernel/devices/timers/pit_timer.hpp>

#include <cx/os/kernel/kprintf.hpp>

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
    // TODO: Fix frequencies!
    // return;
    
    int divisor = detail::kPitTimerFreqBase / (int) frequency;
    
    ports::WriteB(kPitTimerPort_Command, 0x36);
    ports::WriteB(kPitTimerPort_Channel0, divisor & 0xFF);
    ports::WriteB(kPitTimerPort_Channel0, divisor >> 8);
}

cx::os::kernel::timers::PitTimer::~PitTimer()
{
    kprintf("cx::os::kernel::timers::PitTimer destroyed; create a new timer?\n");
}
