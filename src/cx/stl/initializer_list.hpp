//
//  initializer_list.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 04.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef initializer_list_h
#define initializer_list_h
// #include <initializer_list>

// Can't use cx::std here :(
namespace cx::std
{
    // Proudly stolen from libc++, I'm sorry
    template<class _Ep>
    class initializer_list
    {
        const _Ep* __begin_;
        size_t    __size_;
        
        inline
        constexpr
        initializer_list(const _Ep* __b, size_t __s) noexcept
        : __begin_(__b),
        __size_(__s)
        {}
    public:
        typedef _Ep        value_type;
        typedef const _Ep& reference;
        typedef const _Ep& const_reference;
        typedef size_t    size_type;
        
        typedef const _Ep* iterator;
        typedef const _Ep* const_iterator;
        
        inline
        constexpr
        initializer_list() noexcept : __begin_(nullptr), __size_(0) {}
        
        inline
        constexpr
        size_t    size()  const noexcept {return __size_;}
        
        inline
        constexpr
        const _Ep* begin() const noexcept {return __begin_;}
        
        inline
        constexpr
        const _Ep* end()   const noexcept {return __begin_ + __size_;}
    };
    
    template<class _Ep>
    inline
    constexpr
    const _Ep*
    begin(initializer_list<_Ep> __il) noexcept
    {
        return __il.begin();
    }
    
    template<class _Ep>
    inline
    constexpr
    const _Ep*
    end(initializer_list<_Ep> __il) noexcept
    {
        return __il.end();
    }
}

#endif /* initializer_list_h */
