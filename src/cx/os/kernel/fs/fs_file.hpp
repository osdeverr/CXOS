//
//  fs_file.hpp
//  Kernel
//
//  Created by Nikita Ivanov on 08.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#ifndef fs_file_h
#define fs_file_h
#include <cx/os/kernel/fs/fs_node_impl.hpp>
#include <cx/os/kernel/fs/fs_file_stream.hpp>

namespace cx::os::kernel::fs
{
    class FsFile : public FsNodeImpl<FsFile>
    {
    public:
        FsFile(const FsNodeName& name, void* fdata, size_t fsize)
        : FsNodeImpl<FsFile>(name), _fdata(fdata), _fsize(fsize)
        {}
        
        void* GetFileData()
        {
            return _fdata;
        }
        
        const void* GetFileData() const
        {
            return _fdata;
        }
        
        size_t GetFileSize() const
        {
            return _fsize;
        }
        
        bool IsOpen() const
        {
            return _open;
        }
        
        std::shared_ptr<FsFileStream> OpenFileStream()
        {
            if(!_open)
            {
                _open = true;
                return std::make_shared<FsFileStream>(_fdata, _fsize, &_open);
            }
            else
            {
                return nullptr;
            }
        }
        
    private:
        void* _fdata;
        size_t _fsize;
        
        bool _open = false;
        
    public:
        static constexpr auto FsNodeImpl_NodeType = FsNodeType::RegularFile;
    };
}


#endif /* fs_file_h */
