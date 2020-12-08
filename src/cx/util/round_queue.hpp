//
//  round_queue.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef round_queue_h
#define round_queue_h
#include <stddef.h>
#include <cx_throw.hpp>

namespace cx
{
    template<class T, size_t Max>
    class RoundQueue
    {
    public:
        RoundQueue() = default;
        RoundQueue(const RoundQueue&) = default;
        RoundQueue(RoundQueue&&) = default;
        
        bool empty() const
        {
            return _vec.empty();
        }
        
        size_t size() const
        {
            return _vec.size();
        }
        
        static constexpr size_t capacity()
        {
            return Max;
        }
        
        void push(const T& value)
        {
            if(_vec.size() >= Max)
                _vec.clear();
            
            _vec.push_back(value);
        }
        
        T pop()
        {
            auto result = _vec.front();
            _vec.pop_back();
            
            return result;
        }
        
    private:
        FixedVector<T, Max> _vec;
    };
}

#endif /* round_queue_h */
