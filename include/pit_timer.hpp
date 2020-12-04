//
//  pit_timer.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 04.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef pit_timer_h
#define pit_timer_h
#include <timer.hpp>
#include <ports.hpp>

namespace cx::os::kernel::timers
{
    enum PitTimerPort
    {
        kPitTimerPort_Channel0 = 0x40,
        kPitTimerPort_Channel1 = 0x41,
        kPitTimerPort_Channel2 = 0x42,
        kPitTimerPort_Command = 0x43,
    };
    
    enum class PitTimerChannel
    {
        Channel0,
        Channel1,
        Channel2,
        ReadBack,
    };
    
    enum class PitTimerAccessMode
    {
        LatchCountValue,
        LoByte,
        HiByte,
        LoHiByte
    };
    
    enum class PitTimerOperatingMode
    {
        FireIrq,
        OneShot,
        RateGenerator,
        SquareWaveGenerator,
        SoftwareStrobe,
        HardwareStrobe
    };
    
    struct PitTimerCommandSet
    {
        bool bcd_enable : 1;
        PitTimerOperatingMode operating_mode : 3;
        PitTimerAccessMode access_mode : 2;
        PitTimerChannel channel : 2;
    };
    
    
    
    class PitTimer : public Timer
    {
    public:
        PitTimer(TimerFrequency frequency = 18.f);
        ~PitTimer();
        
        virtual void SetFrequencyImpl(TimerFrequency frequency) override;
    };
}

#endif /* pit_timer_h */
