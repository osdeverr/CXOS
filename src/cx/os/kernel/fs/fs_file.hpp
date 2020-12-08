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

namespace cx::os::kernel::fs
{
    class FsFile : public FsNodeImpl<FsFile>
    {
    public:
        FsFile(const FsNodeName& name, void* fdata, size_t fsize)
        : FsNodeImpl<FsFile>(name), _fdata(fdata), _fsize(fsize)
        {}
        
        const void* GetFileData() const
        {
            return _fdata;
        }
        
        size_t GetFileSize() const
        {
            return _fsize;
        }
        
    private:
        void* _fdata;
        size_t _fsize;
        
    public:
        static constexpr auto FsNodeImpl_NodeType = FsNodeType::File;
    };
}


#endif /* fs_file_h */
