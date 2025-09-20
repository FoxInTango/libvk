#ifndef _VULKAN_ANDROID_CONTEXT_H_
#define _VULKAN_ANDROID_CONTEXT_H_
#include <libcpp/libcpp.h>

#ifdef PLATFORM_ANDROID
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
