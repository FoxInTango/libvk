#ifndef _VULKAN_WIN32_CONTEXT_H_
#define _VULKAN_WIN32_CONTEXT_H_

#ifdef PLATFORM_WIN32
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
