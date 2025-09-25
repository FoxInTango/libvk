/**
 * 1,vkEnumerateInstanceExtensionProperties (./base.cpp:createInstance)https://docs.vulkan.org/guide/latest/enabling_extensions.html
 * 2,vkEnumerateInstanceLayerProperties
 * 3,vkCreateInstance
 * 4,vkEnumeratePhysicalDevices (./base.cpp:initVulkan)
 * 5,vkGetPhysicalDeviceProperties [Device Selection]
 * 6,createLogicalDevice
 * 7,vkGetDeviceQueue
 * 8,vkCreateSemaphore 
 * getEnabledFeatures getEnabledExtensions[to be implemented]
 */
#ifndef _LIBVK_APPLICATION_H_
#define _LIBVK_APPLICATION_H_
#include "VKContext.h"
#include <libcpp/libcpp.h>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class foxintangoAPI VKApplication:public VKContext{
public:
    VKApplication();
    virtual ~VKApplication();
};
namespaceEnd
EXTERN_C_END
#endif
