//
//  fs_file_stream.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 10.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_file_stream_h
#define fs_file_stream_h
#include <cx/os/kernel/fs/fs_block_stream.hpp>

namespace cx::os::kernel::fs
{
    class FsFileStream : public IFsBlockStream
    {
    public:
        FsFileStream(void* data, size_t size, bool* open_ptr)
        : _data(data), _size(size), _hack_open_ptr(open_ptr)
        {}
        
        ~FsFileStream()
        {
            if(IsOpen())
                Close();
        }
        
        bool IsOpen() const
        {
            return _open;
        }
        
        void Close()
        {
            _open = false;
            *_hack_open_ptr = _open;
        }
        
        FsStreamDescriptor GetStreamDescriptor() const
        {
            return _fd; // TODO
        }
        
        bool IsEOF() const
        {
            return _eof || !_open;
        }
        
        FsCharacterByte ReadByte()
        {
            if(IsEOF())
                return kFsFileEOF;
            
            auto byte = FsCharacterByte(((char*) _data)[_pos++]);
            if(_pos >= _size)
                _eof = true;
            
            return byte;
        }
        
        void WriteByte(FsCharacterByte)
        {
            // TODO
        }
        
        size_t WriteBuffer(const void* buffer, size_t size)
        {
            // TODO
            return 0;
        }
        
        size_t ReadBuffer(void* buffer, size_t size)
        {
            char* cbuf = (char*) buffer;
            
            size_t count;
            for(count = 0; count < size && !IsEOF(); count++, _pos++)
            {
                *(cbuf++) = (uint8_t) ReadByte();
                
                if(_pos >= _size)
                    _eof = true;
            }
            
            return count;
        }
        
        void Seek(FsSeekMode mode, size_t amount)
        {
            switch(mode)
            {
                case FsSeekMode::FromStart:
                    _pos = amount;
                    break;
                case FsSeekMode::FromCurr:
                    _pos += amount;
                    break;
                case FsSeekMode::FromEnd:
                    _pos = _size - amount;
                    break;
            }
            
            if(_pos >= _size)
                _eof = true;
        }
        size_t Tell() const
        {
            return _pos;
        }
        
    private:
        FsStreamDescriptor _fd = GetAvailableStreamDescriptor();
        
        void* _data;
        size_t _size;
        
        size_t _pos = 0;
        bool _eof = false;
        
        bool _open = true;
        bool* _hack_open_ptr;
    };
}

#endif /* fs_file_stream_h */
