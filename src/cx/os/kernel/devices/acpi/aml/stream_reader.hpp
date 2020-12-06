//
//  stream_reader.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 06.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef stream_reader_h
#define stream_reader_h

namespace cx::os::kernel::acpi::aml
{
    template<class ByteT>
    class StreamReader
    {
    public:
        StreamReader(ByteT* begin, ByteT* end)
        : _begin(begin), _curr(begin), _end(end)
        {}
        
        template<class T>
        T& Read()
        {
            auto ptr = reinterpret_cast<T*>(_curr);
            _curr += sizeof(T);
            return *ptr;
        }
        
        bool IsEof() const
        {
            return _curr >= _end;
        }
        
    private:
        ByteT* _begin;
        ByteT* _curr;
        ByteT* _end;
    };
}

#endif /* stream_reader_h */
