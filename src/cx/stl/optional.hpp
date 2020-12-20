//
//  optional.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 20.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef optional_h
#define optional_h

namespace cx::std
{
    struct nullopt_t
    {};
    
    constexpr static nullopt_t nullopt;
    
    template<class T>
    class optional
    {
    public:
        optional() = default;
        
        optional(nullopt_t)
        {}
        
        optional(const T& value)
        : _present(true)
        {
            _value.object = value;
        }
        
        optional(const optional& other)
        {
            if(other._present)
            {
                this->_present = true;
                this->_value.object = other._value.object;
            }
        }
        
        optional(optional&& other)
        {
            if(other._present)
            {
                this->_present = true;
                this->_value.object = other._value.object;
            }
        }
        
        ~optional()
        {
            if(_present)
                _value.object.~T();
        }
        
        operator bool() const
        {
            return _present;
        }
        
        T& operator*()
        {
            return _value.object;
        }
        
        const T& operator*() const
        {
            return _value.object;
        }
        
        T* operator->()
        {
            return &_value.object;
        }
        
        const T* operator->() const
        {
            return &_value.object;
        }
        
    private:
        union {
            T object;
            char data[sizeof(T)];
        } _value;
        
        bool _present = false;
    };
}

#endif /* optional_h */
