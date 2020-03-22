//
//  tlist.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef tlist_h
#define tlist_h

template<typename T>
struct tList
{
    //static constexpr T* kInvalid = (T*) 0xFFFFFFFF;
    T* Next = (T*) 0xFFFFFFFF;
    T* Prev = (T*) 0xFFFFFFFF;
    tList() : Next((T*) 0xFFFFFFFF), Prev((T*) 0xFFFFFFFF) {}
    
    T* begin() { return Prev->Next; }
    T* end() { return Prev; }
    
    bool Valid() { return Next != (T*) 0xFFFFFFFF && Prev != (T*) 0xFFFFFFFF; }
    
    void Insert(T* value)
    {
        if(!Valid())
            Next = Prev = value;
        
        value->Next = Prev->Next;
        value->Prev = Prev;
        
        Prev->Next = value;
        Prev = value;
    }
};

#endif /* tlist_h */
