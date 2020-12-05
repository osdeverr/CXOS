//
//  enable_if.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 01.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef enable_if_h
#define enable_if_h

namespace cx::std
{
    template<bool Value, class T = void>
    struct enable_if
    {
    };
    
    template<class T>
    struct enable_if<true, T>
    {
        using type = T;
    };
    
    template<bool Value, class T = void>
    using enable_if_t = typename enable_if<Value, T>::type;
}

#endif /* enable_if_h */
