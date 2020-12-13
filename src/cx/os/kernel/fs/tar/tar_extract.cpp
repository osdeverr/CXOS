//
//  tar_extract.cpp
//  Kernel
//
//  Created by Nikita Ivanov on 13.12.2020.
//  Copyright © 2020 CXOS Team. All rights reserved.
//

#include <cx/os/kernel/fs/tar/tar_extract.hpp>
#include <cx/os/kernel/fs/tar/tar_file.hpp>

#include <cx/os/kernel/fs/fs_file.hpp>
#include <cx/os/kernel/fs/fs.hpp>

namespace cx::os::kernel::fs::tar::detail
{
}

bool cx::os::kernel::fs::tar::ExtractTarballToDirectory(TarFile file, FsDirectory& directory)
{
    while(auto header = file.GetNext())
    {
        auto size = header->file_size.Get();
        
        std::list<FsNodeName> names;
        FsNodeName last = "";
        
        const char* curr = header->file_name;
        
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
        
        kprintf("Path of %s: ", header->file_name);
        for(auto& name : names)
            printf("%s -> ", name.AsCharPtr());
        
        printf("\n");
        
        int i = 0;
        auto dir = &directory;
        for(auto& name : names)
        {
            if(i == names.size() - 1)
                break;
            i++;
            
            auto node = dir->FindDirectoryEntry(name);
            // kprintf("Path stuffs: ent='%s' in '%s'; node=0x%08X\n", name.AsCharPtr(), dir->GetName().AsCharPtr(), node);
            
            dir = node->As<fs::FsDirectory>();
        }
        
        // kprintf("Path stuffs: last=%s\n", last.AsCharPtr());
        
        switch(header->file_type)
        {
            case TarFileType::Regular:
                // kprintf("Path stuffs: Adding FsFile @ '%s'\n", last.AsCharPtr());
                dir->AddDirectoryEntry(std::make_shared<fs::FsFile>(last, header->GetFileContents(), size));
                break;
            case TarFileType::Directory:
                // kprintf("Path stuffs: Adding FsDirectory @ '%s'\n", last.AsCharPtr());
                dir->AddDirectoryEntry(std::make_shared<fs::FsDirectory>(last));
                break;
                
            default:
                kprintf("\e[33mTAR => Unpacking: Invalid node type {%d}\e[0m\n", (int) header->file_type);
                return false;
        }
    }
    
    return true;
}
