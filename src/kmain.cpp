#include <kprint.h>
#include <gdt.h>

void CXMain()
{
    GDT::Initialize();
    kprint("PlakOS! Elaymm4! Death! Suffering! :smiley:");
}

extern "C" void kmain(int a, int b)
{
    CXMain();
}
