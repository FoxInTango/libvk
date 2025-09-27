#ifndef _LIBVK_WINDOW_SURFACE_H_
#define _LIBVK_WINDOW_SURFACE_H_
#include "VKSurface.h"
#include <libcpp/libcpp.h>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class VKWindowSurface :public VKSurface{
protected:
VkSurfaceKHR   surfaceKHR   { VK_NULL_HANDLE };
VkSwapchainKHR swapchainKHR { VK_NULL_HANDLE };
protected:
    VKWindowSurface();
public:
    /**
     * If width or height is zero,the window will be fullscreen.
     * */
    VKWindowSurface(uint32_t width,uint32_t height,bool hasTitleBar = false,char* title = "Vulkan Window");
    
    virtual ~VKWindowSurface();
};
namespaceEnd
EXTERN_C_END
#endif
