#include <intrinsics.h>
#include <startup.hpp>
#include <stl/enable_if.hpp>
 
extern "C" void kmain(int b, multiboot_info_t* pMBInfo)
{
    if(pMBInfo == nullptr)
        return;
    
    cx::os::kernel::BeginKernelStartup(*pMBInfo);
}
