#ifndef _LIBVK_CONTEXT_H_
#define _LIBVK_CONTEXT_H_
#ifdef PLATFORM_LINUX
#define VK_USE_PLATFORM_WAYLAND_KHR
//#define VK_USE_PLATFORM_DIRECTFB_EXT
#endif
#include "VKDisplay.h"
#include "VKDevice.h"
#include "VKBuffer.h"
#include <libcpp/libcpp.h>
#include <vulkan/vulkan.h>
#include <string>
#include <map>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
/** VKDisplaySurfaces should be created by VKContext Only.
 * */
class foxintangoAPI VKContext{
friend class VKDevice;
friend class VKSurface;
friend class VKTexture;
friend class VKDisplaySurface;
friend class VKHeadlessSurface;
protected:
    std::map<std::string,VKDevice*>         deviceMap;
    VkInstance vulkan;
public:
    VKContext();
    virtual ~VKContext();
public:
    void clean();
public:
    /** Physical GPU Device Methods
     * */
    void enumeratePhysicalDevices();
    uint32_t                          physicalDeviceCount();
    VkPhysicalDevice                  physicalDeviceAt(uint32_t index);
    VkPhysicalDevice                  physicalDeviceAt(const char* name);
    VkPhysicalDeviceProperties*       physicalDevicePropertiesOf(uint32_t index);
    VkPhysicalDeviceProperties*       physicalDevicePropertiesOf(const char* name);
    VkPhysicalDeviceFeatures*         physicalDeviceFeaturesOf(uint32_t index);
    VkPhysicalDeviceFeatures*         physicalDeviceFeaturesOf(const char* name);
    VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryPropertiesOf(uint32_t index);
    VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryPropertiesOf(const char* name);
    /** Physical Display methods
     *      Implemented in platform/VK{Platform}.cpp
     * */
    uint32_t                     physicalDisplayPropertyCount();
    VkDisplayPropertiesKHR*      physicalDisplayPropertiesAt(uint32_t index);
    VkDisplayPlanePropertiesKHR* physicalDisplayPlanPropertiesAt(uint32_t index);
    /** Display Methods
     *      Implemented in platform/VK{Platform}.cpp
     * */
    void enumerateDisplays();
    uint32_t   displayCount();
    VKDisplay* displayAt(uint32_t index);
    VKDisplay* displayAt(const char* name);

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
public:
    VKTexture*         loadTextureFromFile(VKDevice*   targetDevice,char* path);
    VKTexture*         loadTextureFromMemory(VKDevice* targetDevice,char* data,uint32_t width,uint32_t height,VkFormat format);
    //VKTextureSurface*  createTextureSurface();
    //VKDisplaySurface*  createDisplaySurface(VKDevice*  targetDevice,uint32_t width,uint32_t height,bool hasTitleBar = false,char* title = "Vulkan Window");
    VKDisplaySurface*  createDisplaySurface(VKDisplay* display,uint32_t width,uint32_t height,bool hasTitleBar = false,char* title = "Vulkan Window");
    VKDisplaySurface*  createDisplaySurface(char*  displayName,uint32_t width,uint32_t height,bool hasTitleBar = false,char* title = "Vulkan Window");
    VKHeadlessSurface* createHeadlessSurface(uint32_t width,uint32_t height,char* deviceName);
    VKHeadlessSurface* createHeadlessSurface(uint32_t width,uint32_t height,VKDevice* device);
public:
    virtual void enableInstanceExtensions();
    virtual void enableDeviceFeatures();
    virtual void enableDeviceExtensions();
};
namespaceEnd
EXTERN_C_END
#endif
