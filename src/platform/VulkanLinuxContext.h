#ifndef _VULKAN_LINUX_CONTEXT_H_
#define _VULKAN_LINUX_CONTEXT_H_

#ifdef PLATFORM_LINUX
#include <libcpp/libcpp.h>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class foxintangoAPI VulkanContext{
public:
    VulkanContext();
    virtual ~VulkanContext();

};
namespaceEnd
EXTERN_C_END
#endif
#endif
