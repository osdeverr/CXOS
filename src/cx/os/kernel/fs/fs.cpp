//
//  fs.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 09.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/fs/fs.hpp>
#include <cx/stl/list.hpp>

namespace cx::os::kernel::fs::detail
{
    // No name and no parents
    FsDirectory gTheFsRoot = {"", nullptr};
    
    struct OpenedFileStream
    {
        FsStreamDescriptor fd = 0;
        
        std::shared_ptr<IFsCharacterStream> char_stream = nullptr;
        std::shared_ptr<IFsBlockStream> block_stream = nullptr;
    };
    
    bool gOpenedFilesInitialized = false;
    std::list<OpenedFileStream> gOpenedFiles;
}

cx::os::kernel::fs::FsDirectory& cx::os::kernel::fs::GetFilesystemRoot()
{
    return detail::gTheFsRoot;
}

cx::std::shared_ptr<cx::os::kernel::fs::FsNode> cx::os::kernel::fs::FindFilesystemNode(const char* path)
{
    std::list<FsNodeName> names;
    FsNodeName last = "";
    
    const char* curr = path;
    if(*curr == '/')
        curr++;
    
    while(*curr != '\0')
    {
        const char* start = curr;
        while(*curr != '/' && *curr != '\0')
            curr++;
        
        auto name = FsNodeName{start, (size_t) (curr - start)};
        
        if(*curr == '/')
            curr++;
        
        names.push_back(name);
        last = name;
    }
    
    int i = 0;
    auto dir = &GetFilesystemRoot();
    for(auto& name : names)
    {
        if(i == names.size() - 1)
            break;
        i++;
        
        auto node = dir->FindDirectoryEntry(name);
        
        dir = node->As<fs::FsDirectory>();
        if(!dir)
            return nullptr;
    }
    
    return dir->FindDirectoryEntry(last);
}

cx::std::shared_ptr<cx::os::kernel::fs::IFsCharacterStream> cx::os::kernel::fs::OpenCharacterStream(const char* path)
{
    if(auto node = FindFilesystemNode(path))
    {
        auto stream = node->OpenCharacterStream();
        detail::gOpenedFiles.push_back({stream->GetStreamDescriptor(), stream, nullptr});
        return stream;
    }
    else
    {
        return nullptr;
    }
}

cx::std::shared_ptr<cx::os::kernel::fs::IFsBlockStream> cx::os::kernel::fs::OpenBlockStream(const char* path)
{
    if(auto node = FindFilesystemNode(path))
    {
        auto stream = node->OpenBlockStream();
        
        // Hangs the system if we don't do this: blame static constructors..
        if(!detail::gOpenedFilesInitialized)
        {
            detail::gOpenedFiles = {};
            detail::gOpenedFilesInitialized = true;
        }
        
        detail::gOpenedFiles.push_back({stream->GetStreamDescriptor(), stream, stream});
        return stream;
    }
    else
    {
        return nullptr;
    }
}

cx::os::kernel::fs::FsStreamDescriptor cx::os::kernel::fs::GetAvailableStreamDescriptor()
{
    FsStreamDescriptor result = 1;
    for(auto& file : detail::gOpenedFiles)
        if(file.fd == result)
            result++;
    
    return result;
}
