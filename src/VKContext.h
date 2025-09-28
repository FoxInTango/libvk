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
#include <string>
#include <map>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class foxintangoAPI VKContext{
friend class VKDevice;
friend class VKSurface;
friend class VKTexture;
friend class VKWindowSurface;
friend class VKHeadlessSurface;
protected:
    std::map<std::string,VKDevice*>         deviceMap;
    std::map<std::string,VkPhysicalDevice>  physicalDeviceMap;
    VkInstance vulkan;
public:
    VKContext();
    virtual ~VKContext();
public:
    void clean();
public:
    void enumeratePhysicalDevices();
    void enumerateDisplay();
    VKDevice* createDevice(VkPhysicalDevice           physicalDevice,
                           VkPhysicalDeviceFeatures   enabledFeatures,
                           std::vector<const char *>  enabledExtensions,
                           void*                      pNextChain,
                           bool                       useSwapChain        = true,
                           VkQueueFlags               requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
                           const char*                name                = nullptr);
    VKDevice* createDevice(const char*                physicalDeviceName,
                           VkPhysicalDeviceFeatures   enabledFeatures,
                           std::vector<const char *>  enabledExtensions,
                           void*                      pNextChain,
                           bool                       useSwapChain        = true,
                           VkQueueFlags               requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
                           const char*                name                = nullptr);
    void destroyDevice(VKDevice*   device);
    void destroyDevice(const char* name);
    void destroyDevices();
public:
    VKDevice*        deviceAt(const char* name);
    VkPhysicalDevice physicalDeviceAt(const char* name);
public:
    VKTexture*         createTextureFromFile(VKDevice*   targetDevice,char* path);
    VKTexture*         createTextureFromMemory(VKDevice* targetDevice,char*    data, uint32_t width, uint32_t height,             VkFormat format);
    VKWindowSurface*   createWindowSurface(VKDevice*     targetDevice,uint32_t width,uint32_t height,bool     hasTitleBar = false,char* title = "Vulkan Window");
    VKHeadlessSurface* createHeadlessSurface(VKDevice*   targetDevice,uint32_t width,uint32_t height);
public:
    virtual void enableInstanceExtensions();
    virtual void enableDeviceFeatures();
    virtual void enableDeviceExtensions();
};
namespaceEnd
EXTERN_C_END
#endif
