#ifndef _VULKAN_DARWIN_CONTEXT_H_
#define _VULKAN_DARWIN_CONTEXT_H_

#ifdef PLATFORM_DARWIN
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
