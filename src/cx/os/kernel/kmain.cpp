#include <cx/os/kernel/intrinsics.h>
#include <cx/os/kernel/startup.hpp>
#include <cx/os/kernel/debug/panic.hpp>

extern "C" void cx_os_kernel_main(int b, multiboot_info_t* pMBInfo)
{
    if(pMBInfo == nullptr)
        return; 
    
    cx::os::kernel::BeginKernelStartup(*pMBInfo);
    cx::os::kernel::debug::TriggerKernelPanic("Kernel looper quit");
}
