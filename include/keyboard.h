//
//  keyboard.h
//  cxkrnltest
//
//  Created by Nikita Ivanov on 3/22/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef keyboard_h
#define keyboard_h

namespace Keyboard
{
    template<typename T, int N>
    class Queue
    {
    public:
        Queue() : NumItems(0) {}
        
        void Push(const T val) { if(NumItems < N) Data[NumItems++] = val; };
        const T Pop() { T val; if(NumItems > 0) NumItems--; return val; };
        void Clear() { NumItems = 0; };
        
        bool Empty() { return NumItems == 0; };
    private:
        T Data[N];
        int NumItems;
    };
    
    void Initialize();
    char ReadChar();
}

#endif /* keyboard_h */
