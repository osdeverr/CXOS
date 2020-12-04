//
//  shared_ptr.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 04.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef shared_ptr_h
#define shared_ptr_h
#include <stddef.h>
#include <stl/nullptr.hpp>

#include <kprintf.hpp>

namespace cx::std
{
    namespace detail
    {
        struct ISharedDeleter
        {
            virtual ~ISharedDeleter() = default;
        };
        
        template<class T>
        struct SharedControlBlock : ISharedDeleter
        {
            SharedControlBlock(T* ptr)
            : ptr(ptr)
            {}
            
            SharedControlBlock(const SharedControlBlock&) = delete;
            SharedControlBlock(SharedControlBlock&&) = delete;
            
            ~SharedControlBlock()
            {
                delete ptr;
            }
            
            size_t refcount = 1;
            T* const ptr;
        };
    }
    
    template<class T>
    class shared_ptr
    {
    public:
        shared_ptr() = default;
        
        shared_ptr(nullptr_t)
        {}
        
        explicit shared_ptr(T* ptr)
        {
            _control = new detail::SharedControlBlock<T>(ptr);
        }
        
        /// FIXME: We have to check IsConvertible or something
        template<class U>
        shared_ptr(const shared_ptr<U>& other)
        {
            AttachControlBlock(reinterpret_cast<detail::SharedControlBlock<T>*>(other._control));
        }
        
        shared_ptr(const shared_ptr& other)
        {
            AttachControlBlock(other._control);
        }
        
        shared_ptr& operator=(const shared_ptr& other)
        {
            AttachControlBlock(other._control);
            return *this;
        }
        
        shared_ptr(shared_ptr&& other)
        {
            AttachControlBlock(other._control);
            other.DetachControlBlock();
        }
        
        ~shared_ptr()
        {
            DetachControlBlock();
        }
        
        operator bool() const
        {
            return _control != nullptr;
        }
        
        T& operator*()
        {
            return *_control->ptr;
        }
        
        const T& operator*() const
        {
            return *_control->ptr;
        }
        
        T* operator->()
        {
            return _control->ptr;
        }
        
        const T* operator->() const
        {
            return _control->ptr;
        }
        
    private:
        void AttachControlBlock(detail::SharedControlBlock<T>* control)
        {
            if(control)
                DetachControlBlock();
            
            _control = control;
            _control->refcount++;
        }
        
        void DetachControlBlock()
        {
            if(!_control)
                return;
            
            _control->refcount--;
            if(_control->refcount < 1)
                delete _control;
            
            _control = nullptr;
        }
        
        template<class U>
        friend class shared_ptr;
        
        detail::SharedControlBlock<T>* _control = nullptr;
    };
    
    template<class T, class... Args>
    shared_ptr<T> make_shared(Args... args)
    {
        return shared_ptr<T>(new T(args...));
    }
}

#endif /* shared_ptr_h */
