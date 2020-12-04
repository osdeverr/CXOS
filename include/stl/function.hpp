//
//  function.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 04.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef function_h
#define function_h
#include <stl/shared_ptr.hpp>

namespace cx::std
{
    namespace detail
    {
        template<class R, class... Args>
        class IInvokeable
        {
        public:
            virtual ~IInvokeable() = default;
            
            virtual R Invoke(Args... args) const = 0;
        };
        
        template<class F, class R, class... Args>
        class InvokeableImpl : public IInvokeable<R, Args...>
        {
        public:
            /*
                -- Need std::move()
             
                InvokeableImpl(F&& func)
                : _func(std::move(func))
                {}
             */
            
            InvokeableImpl(const F& func)
            : _func(func)
            {}
            
            virtual R Invoke(Args... args) const override
            {
                return _func(args...);
            }
            
        private:
            F _func;
        };
    }
    
    template<class R, class... Args>
    class function;
    
    template<class R, class... Args>
    class function<R(Args...)>
    {
    public:
        function() = default;
        function(const function&) = default;
        function(function&&) = default;
        
        template<class F>
        function(const F& func)
        : _cb(make_shared<detail::InvokeableImpl<F, R, Args...>>(func))
        {}
        
        R operator()(Args... args) const
        {
            return _cb->Invoke(args...);
        }
        
    private:
        shared_ptr<detail::IInvokeable<R, Args...>> _cb = nullptr;
    };
}

#endif /* function_h */
