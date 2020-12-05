//
//  fixed_vector.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 03.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef fixed_vector_h
#define fixed_vector_h
#include <stddef.h>
#include <cx_throw.hpp>
#include <cx/stl/initializer_list.hpp>

namespace cx
{
    template<class T>
    class FixedVectorIterator
    {
    public:
        FixedVectorIterator<T>(T* ptr, T* end)
        : _ptr(ptr), _end(end)
        {}
        
        FixedVectorIterator<T>& operator++()
        {
            if(_ptr == _end)
                CX_THROW_FATAL("cx::FixedVectorIterator<T>::operator++ => attempting to increment an end iterator");
            
            _ptr++;
            return *this;
        }
        
        FixedVectorIterator<T>& operator++(int)
        {
            return (*this)++;
        }
        
        T& operator*()
        {            
            if(_ptr == _end)
                CX_THROW_FATAL("cx::FixedVectorIterator<T>::operator* => attempting to dereference an end iterator");
            
            return *_ptr;
        }
        
        T* operator->()
        {
            if(_ptr == _end)
                CX_THROW_FATAL("cx::FixedVectorIterator<T>::operator-> => attempting to dereference an end iterator");
            
            return _ptr;
        }
        
        bool operator==(const FixedVectorIterator<T>& other) const
        {
            return this->_ptr == other._ptr && this->_end == other._end;
        }
        
        bool operator!=(const FixedVectorIterator<T>& other) const
        {
            return !(*this == other);
        }
        
    private:
        T* _ptr;
        T* _end;
    };
    
    template<class T, size_t Max>
    class FixedVector
    {
    public:
        using value_type = T;
        using iterator = FixedVectorIterator<T>;
        using const_iterator = FixedVectorIterator<const T>;
        
        FixedVector() = default;
        FixedVector(const FixedVector&) = default;
        FixedVector(FixedVector&&) = default;
        
        template<class A>
        FixedVector(const A& arr)
        {
            for(auto& v : arr)
                push_back(v);
        }
        
        FixedVector(std::initializer_list<T> ilist)
        {
            for(auto& v : ilist)
                push_back(v);
        }
        
        iterator begin()
        {
            return CreateIterator(_items[0]);
        }
        
        const_iterator begin() const
        {
            return CreateIterator(_items[0]);
        }
        
        iterator end()
        {
            return CreateIterator(_items[_count]);
        }
        
        const_iterator end() const
        {
            return CreateIterator(_items[_count]);
        }
        
        size_t size() const
        {
            return _count;
        }
        
        static constexpr size_t capacity()
        {
            return Max;
        }
                
        iterator find(const value_type& value)
        {
            for(auto& v : *this)
                if(v == value)
                    return CreateIterator(v);
            
            return end();
        }
        
        template<class F>
        iterator find_if(F&& pred)
        {
            for(auto& v : *this)
                if(pred(v))
                    return CreateIterator(v);
            
            return end();
        }
        
        value_type& operator[](size_t index)
        {
            return _items[index];
        }
        
        iterator push_back(const value_type& value)
        {
            if(size() == Max)
                CX_THROW_FATAL("cx::FixedVector<T, Max>::push_back => vector storage full");
            
            auto& ref = _items[_count++];
            ref = value;
            return CreateIterator(ref);
        }
        
        void clear()
        {
            for(auto& value : *this)
                value = {};
            
            _count = 0;
        }
        
    private:
        value_type _items[Max] = {};
        size_t _count = 0;
        
        iterator CreateIterator(value_type& ref)
        {
            return iterator{&ref, &_items[_count]};
        }
        
        const_iterator CreateIterator(const value_type& ref) const
        {
            return const_iterator{&ref, &_items[_count]};
        }
    };
}

#endif /* fixed_vector_h */
