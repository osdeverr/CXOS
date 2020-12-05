//
//  pair.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 05.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef pair_h
#define pair_h

namespace cx::std
{
    template<class T1, class T2>
    struct pair
    {
        T1 first;
        T2 second;
    };
}

#endif /* pair_h */
