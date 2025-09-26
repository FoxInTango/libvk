#ifndef _LIBVK_CONTEXT_H_
#define _LIBVK_CONTEXT_H_
#ifdef PLATFORM_LINUX
#define VK_USE_PLATFORM_WAYLAND_KHR
//#define VK_USE_PLATFORM_DIRECTFB_EXT
#endif
#include "VKDevice.h"
#include "VKBuffer.h"
#include <libcpp/libcpp.h>
#include <vulkan/vulkan.h>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class foxintangoAPI VKContext{
protected:
    foxintango::VKDevice   device;
    VkInstance vkInstance;
public:
    VKContext();
    virtual ~VKContext();
public:
    virtual void enableFeatures();
    virtual void enableExtensions();
};
namespaceEnd
EXTERN_C_END
#endif
