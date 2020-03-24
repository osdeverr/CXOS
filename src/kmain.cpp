#include <console.h>
#include <gdt.h>
#include <idt.h>
#include <multiboot.h>
#include <memory.h>
#include <assert.h>
#include <itoa.h>
#include <ioports.h>
#include <keyboard.h>
#include <fs/root.h>
#include <tlist.h>

typedef struct multiboot_memory_map {
    unsigned int size;
    unsigned int base_addr_low,base_addr_high;
    // You can also use: unsigned long long int base_addr; if supported.
    unsigned int length_low,length_high;
    // You can also use: unsigned long long int length; if supported.
    unsigned int type;
} multiboot_memory_map_t;

// this is really an entry, not the entire map.
typedef multiboot_memory_map_t mmap_entry_t;

class Plak
{
public:
    Plak() : msg("ky & plaks\n") {}
    void Show() { Console::Write(msg); }
private:
    const char* msg = nullptr;
};

void CXMain(multiboot_info_t* mbt)
{
    Console::Initialize();
    GDT::Initialize();
    IDT::Initialize();
    Keyboard::Initialize();
    
    Console::Write("Yers\n");
    mmap_entry_t* entry = (mmap_entry_t*) mbt->mmap_addr;\
    
    while((unsigned int) entry < mbt->mmap_addr + mbt->mmap_length) {
        if(entry->type == 1)
        {            
            Memory::AddRegion((void*) entry->base_addr_low, entry->length_low);
        }
        entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    }
    Plak* pPlak = new Plak;
    pPlak->Show();
    
    IDT::AddFaultHandler(IDT::eFault_Breakpoint,
        [](const IDT::RegisterState& regs) {
            Panic::Halt("Breakpoint hit in kernel");
        }
    );

    while(1)
        Console::Write(uint8_t(Keyboard::ReadChar()));
}

extern "C" void kmain(int b, multiboot_info_t* pMBInfo)
{
    CXMain(pMBInfo);
}
