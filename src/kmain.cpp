#include <intrinsics.h>
#include <startup.hpp>
#include <panic.hpp>

extern "C" void kmain(int b, multiboot_info_t* pMBInfo)
{
    if(pMBInfo == nullptr)
        return; 
    
    cx::os::kernel::BeginKernelStartup(*pMBInfo);
    cx::os::kernel::debug::TriggerKernelPanic("Kernel looper quit");
}
