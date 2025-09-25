#ifndef _LIBVK_CONTEXT_H_
#define _LIBVK_CONTEXT_H_
#include <libcpp/libcpp.h>
#ifdef PLATFORM_LINUX
#define VK_USE_PLATFORM_WAYLAND_KHR
//#define VK_USE_PLATFORM_DIRECTFB_EXT
#endif
#include <vulkan/vulkan.h>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class foxintangoAPI VKContext{
protected:
    VkInstance vkInstance;
public:
    VKContext();
    virtual ~VKContext();
};
namespaceEnd
EXTERN_C_END
#endif
