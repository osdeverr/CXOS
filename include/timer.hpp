//
//  timer.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 04.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef timer_h
#define timer_h
#include <fixed_vector.hpp>
#include <stl/function.hpp>

namespace cx::os::kernel::timers
{
    using TimerFireCallback = std::function<void()>;
    using TimerCallbackList = FixedVector<TimerFireCallback, 32>;
    using TimerFrequency = float;
    
    class Timer
    {
    public:
        virtual ~Timer() = default;
        
        Timer() = default;
        Timer(const Timer&) = delete;
        Timer(Timer&&) = delete;
        
        Timer(TimerFrequency frequency)
        {
            SetFrequency(frequency);
        }
        
        void SetFrequency(TimerFrequency frequency)
        {
            _frequency = frequency;
            SetFrequencyImpl(frequency);
        }
        
        TimerFrequency GetFrequency() const
        {
            return _frequency;
        }
        
        void AddFireCallback(TimerFireCallback callback)
        {
            _callbacks.push_back(callback);
        }
        
    protected:
        virtual void SetFrequencyImpl(TimerFrequency) = 0;
        
    protected:
        TimerCallbackList _callbacks;
        TimerFrequency _frequency = 0;
    };
}

#endif /* timer_h */
