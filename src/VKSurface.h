#ifndef _LIBVK_SURFACE_H_
#define _LIBVK_SURFACE_H_
#include <libcpp/libcpp.h>
#include <vulkan/vulkan.h>
#include <vector>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
enum VKSurfaceType{
TEXTURE_SURFACE,
HEADLESS_SURFACE,
WINDOW_SURFACE,
#ifdef PLATFORM_LINUX
DISPLAY_SURFACE
#endif
};
class VKDevice;
/** 
 * One VkCommandBuffer per VKSurface image/imageView with vkCommandPool from VKDevice
 * One VkFramebuffer   per VKSurface image/imageView
 * */
class foxintangoAPI VKSurface{
protected:
    VKDevice*                device{ nullptr };
    VkFormat                 colorFormat{};
    VkColorSpaceKHR          colorSpace{};
    std::vector<VkImage>     images{};
    std::vector<VkImageView> imageViews{};
protected:
    VKSurface();
    virtual ~VKSurface();
};

namespaceEnd
EXTERN_C_END
#endif
