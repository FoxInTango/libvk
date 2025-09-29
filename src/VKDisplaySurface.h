#ifndef _LIBVK_WINDOW_SURFACE_H_
#define _LIBVK_WINDOW_SURFACE_H_
#include "VKSurface.h"
#include <libcpp/libcpp.h>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
/** VKDisplaySurfaces should be created by VKContext Only.
 * */
class VKDisplaySurface :public VKSurface{
protected:
VkSurfaceKHR   surfaceKHR   { VK_NULL_HANDLE };
VkSwapchainKHR swapchainKHR { VK_NULL_HANDLE };
protected:
    VKDisplaySurface();
    /**
     * If width or height is zero,the window will be fullscreen.
     * */
    VKDisplaySurface(uint32_t width,uint32_t height,bool hasTitleBar = false,char* title = "Vulkan Window");
    
    virtual ~VKDisplaySurface();
private:
    void setupSwapchain(uint32_t& width, uint32_t& height,bool vsync, bool fullscreen);
    void clean();
    VkResult queuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore);
    VkResult acquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t& imageIndex);
};
namespaceEnd
EXTERN_C_END
#endif
