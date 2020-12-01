//
//  fsmain.cpp
//  kernel-codebase
//
//  Created by Nikita Ivanov on 3/24/20.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#include <fs/root.h>
#include <fs/directory.h>
#include <fs/file.h>
#include <console.h>
#include <assert.h>

FS::Node* FS::gRoot = nullptr;

void FS::Directory::Debug_Printout(int tabs)
{
    return;
    
    for(int i = 0; i < tabs; i++)
        Console::Write("    ");
    Console::Write(mName);
    Console::Write("/\n");
    if(!mChildren.Valid())
    {
        for(int i = 0; i < tabs + 1; i++)
            Console::Write("    ");
        Console::Write("<empty>\n");
        return;
    }
    
    Node* pNode = mChildren.Prev->Next;
    do
    {
        if(pNode->Type() != eNodeType_Directory)
        {
            for(int i = 0; i < tabs + 1; i++)
                Console::Write("    ");
            Console::Write(pNode->Name());
            Console::Write("\n");
        }
        else
            pNode->As<Directory>()->Debug_Printout(tabs+1);
        pNode = pNode->Next;
    } while(pNode != mChildren.Prev);
}

void FS::Initialize()
{
    Directory* dir = new Directory("");
    dir->Add(new File("test.elf"));
    dir->Add(new File("test2.elf"));
    
    /*
    Directory *dir2 = new Directory("dev");
    dir2->Add(new File("test66.elf"));
    dir2->Add(new File("test77.elf"));
    
    dir->Add(dir2);
    dir->Add(new Directory("test156"));
    //gRoot->Insert(new File("test2.elf"));
     */
    
    // dir->Debug_Printout();
    gRoot = dir;
}
