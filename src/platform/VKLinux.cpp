#include "../VKWindowSurface.h"
using namespace foxintango;
#include <wayland-client.h>
//#include "xdg-shell-client-protocol.h"
#include <assert.h>
/**
 * 1,First we check if there's a wayland compositor running.Create a wayland vulkan surface if yes.
 * 2,Then we check if there's a x-server running.Create a xcb or xlib vulkan surface if yes.
 * 3,Try to create a display surface.
 * */

#ifdef PLATFORM_LINUX
#include "../VKContext.h"
bool has_wayland(){ return false; }
bool has_xserver(){ return false; }
void createDisplaySurface(VkInstance instance,VkPhysicalDevice physicalDevice,VkSurfaceKHR& surfaceKHR,uint32_t width, uint32_t height);

VKWindowSurface::VKWindowSurface(){}
VKWindowSurface::VKWindowSurface(uint32_t width,uint32_t height,bool hasTitleBar,char* title){
    assert(this->device);
    VkResult res;
    if(has_wayland()){ 
/*
    VkWaylandSurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.display = display;
    surfaceCreateInfo.surface = window;
    res = vkCreateWaylandSurfaceKHR(this->context->vkInstance, &surfaceCreateInfo, nullptr, &surfaceKHR);
*/
    return; }
    if(has_xserver()){ 
/*      VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.connection = connection;
        surfaceCreateInfo.window = window;
        res = vkCreateXcbSurfaceKHR(this->context->vkInstance, &surfaceCreateInfo, nullptr, &surface);
*/
    return; }

    //create display surface
    createDisplaySurface(this->device->context->vulkan,this->device->physicalDevice,this->surfaceKHR,width,height);
/*
    if(res != VK_SUCCESS) {
        vks::tools::exitFatal("Could not create surface!", err);
    }
*/
    // Get available queue family properties
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(this->device->physicalDevice, &queueCount, NULL);
    assert(queueCount >= 1);

    std::vector<VkQueueFamilyProperties> queueProps(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(this->device->physicalDevice, &queueCount, queueProps.data());

    // Iterate over each queue to learn whether it supports presenting:
    // Find a queue with present support
    // Will be used to present the swap chain images to the windowing system
    std::vector<VkBool32> supportsPresent(queueCount);
    for(uint32_t i = 0; i < queueCount; i++){
        vkGetPhysicalDeviceSurfaceSupportKHR(this->device->physicalDevice, i,this->surfaceKHR, &supportsPresent[i]);
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;
    for(uint32_t i = 0; i < queueCount; i++){
        if((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0){
            if(graphicsQueueNodeIndex == UINT32_MAX){
                graphicsQueueNodeIndex = i;
            }
            if(supportsPresent[i] == VK_TRUE){
                graphicsQueueNodeIndex = i;
                presentQueueNodeIndex  = i;
                break;
            }
        }
    }

    if(presentQueueNodeIndex == UINT32_MAX){	
        // If there's no queue that supports both present and graphics
        // try to find a separate present queue
        for(uint32_t i = 0; i < queueCount; ++i){
            if(supportsPresent[i] == VK_TRUE){
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    // Exit if either a graphics or a presenting queue hasn't been found
    if(graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX){
        vks::tools::exitFatal("Could not find a graphics and/or presenting queue!", -1);
    }

    if(graphicsQueueNodeIndex != presentQueueNodeIndex){
        vks::tools::exitFatal("Separate graphics and presenting queues are not supported yet!", -1);
    }

    // queueNodeIndex = graphicsQueueNodeIndex;

    // Get list of supported surface formats
    uint32_t formatCount;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(this->device->physicalDevice,this->surfaceKHR,&formatCount,NULL));
    assert(formatCount > 0);

    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(this->device->physicalDevice,this->surfaceKHR,&formatCount,surfaceFormats.data()));
    // We want to get a format that best suits our needs, so we try to get one from a set of preferred formats
    // Initialize the format to the first one returned by the implementation in case we can't find one of the preffered formats
    VkSurfaceFormatKHR selectedFormat = surfaceFormats[0];
    std::vector<VkFormat> preferredImageFormats = { 
        VK_FORMAT_B8G8R8A8_UNORM,
        VK_FORMAT_R8G8B8A8_UNORM, 
        VK_FORMAT_A8B8G8R8_UNORM_PACK32 
    };

    for(auto& availableFormat : surfaceFormats) {
        if (std::find(preferredImageFormats.begin(), preferredImageFormats.end(), availableFormat.format) != preferredImageFormats.end()) {
            selectedFormat = availableFormat;
            break;
        }
    }

    colorFormat = selectedFormat.format;
    colorSpace  = selectedFormat.colorSpace;

}
VKWindowSurface::~VKWindowSurface(){}
/**
* Create direct to display surface
*/	
void createDisplaySurface(VkInstance instance,VkPhysicalDevice physicalDevice,VkSurfaceKHR& surfaceKHR,uint32_t width, uint32_t height)
{
	uint32_t displayPropertyCount;
		
	// Get display property
	vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &displayPropertyCount, NULL);
	printf("displayPropertyCount: %u\n",displayPropertyCount);
        	
	VkDisplayPropertiesKHR* pDisplayProperties = new VkDisplayPropertiesKHR[displayPropertyCount];
	vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, &displayPropertyCount, pDisplayProperties);

	// Get plane property
	uint32_t planePropertyCount;
	vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &planePropertyCount, NULL);
        printf("planePropertyCount: %u\n",planePropertyCount);
	VkDisplayPlanePropertiesKHR* pPlaneProperties = new VkDisplayPlanePropertiesKHR[planePropertyCount];
	vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, &planePropertyCount, pPlaneProperties);

	VkDisplayKHR display = VK_NULL_HANDLE;
	VkDisplayModeKHR displayMode;
	VkDisplayModePropertiesKHR* pModeProperties;
	bool foundMode = false;
         
	for(uint32_t i = 0; i < displayPropertyCount;++i)
	{
		display = pDisplayProperties[i].display;
		uint32_t modeCount;
		vkGetDisplayModePropertiesKHR(physicalDevice, display, &modeCount, NULL);
		pModeProperties = new VkDisplayModePropertiesKHR[modeCount];
		vkGetDisplayModePropertiesKHR(physicalDevice, display, &modeCount, pModeProperties);
		if( 1 > modeCount) {
		    printf("no mode found for display %d with modeCount: %u\n",i,modeCount);
		} else printf("modeCount : %u \n",modeCount);
		for (uint32_t j = 0; j < modeCount; ++j)
		{
			const VkDisplayModePropertiesKHR* mode = &pModeProperties[j];
                        // to be delete : sparrow
			printf("mode %d width: %d height: %d\n",j,
					mode->parameters.visibleRegion.width,
					mode->parameters.visibleRegion.height);
			if (mode->parameters.visibleRegion.width == width && mode->parameters.visibleRegion.height == height)
			{
				displayMode = mode->displayMode;
				foundMode = true;
				break;
			}
		}

		if (foundMode)
		{
			break;
		}
		delete [] pModeProperties;
	}

	if(!foundMode)
	{
		vks::tools::exitFatal("Can't find a display and a display mode!", -1);
		return;
	}

	// Search for a best plane we can use
	uint32_t bestPlaneIndex = UINT32_MAX;
	VkDisplayKHR* pDisplays = NULL;
	for(uint32_t i = 0; i < planePropertyCount; i++)
	{
		uint32_t planeIndex=i;
		uint32_t displayCount;
		vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, &displayCount, NULL);
		if (pDisplays)
		{
			delete [] pDisplays;
		}
		pDisplays = new VkDisplayKHR[displayCount];
		vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, &displayCount, pDisplays);

		// Find a display that matches the current plane
		bestPlaneIndex = UINT32_MAX;
		for(uint32_t j = 0; j < displayCount; j++)
		{
			if(display == pDisplays[j])
			{
				bestPlaneIndex = i;
				break;
			}
		}
		if(bestPlaneIndex != UINT32_MAX)
		{
			break;
		}
	}

	if(bestPlaneIndex == UINT32_MAX)
	{
		vks::tools::exitFatal("Can't find a plane for displaying!", -1);
		return;
	}
	VkDisplayPlaneCapabilitiesKHR planeCap;
	vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, displayMode, bestPlaneIndex, &planeCap);
	VkDisplayPlaneAlphaFlagBitsKHR alphaMode = (VkDisplayPlaneAlphaFlagBitsKHR)0;

	if (planeCap.supportedAlpha & VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR)
	{
		alphaMode = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR;
	}
	else if (planeCap.supportedAlpha & VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR)
	{
		alphaMode = VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR;
	}
	else if (planeCap.supportedAlpha & VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR)
	{
		alphaMode = VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR;
	}
	else if (planeCap.supportedAlpha & VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR)
	{
		alphaMode = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR;
	}

	VkDisplaySurfaceCreateInfoKHR surfaceInfo{};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = NULL;
	surfaceInfo.flags = 0;
	surfaceInfo.displayMode = displayMode;
	surfaceInfo.planeIndex = bestPlaneIndex;
	surfaceInfo.planeStackIndex = pPlaneProperties[bestPlaneIndex].currentStackIndex;
	surfaceInfo.transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	surfaceInfo.globalAlpha = 1.0;
	surfaceInfo.alphaMode = alphaMode;
	surfaceInfo.imageExtent.width = width;
	surfaceInfo.imageExtent.height = height;

	VkResult result = vkCreateDisplayPlaneSurfaceKHR(instance, &surfaceInfo, NULL, &surfaceKHR);
	if (result !=VK_SUCCESS) {
		vks::tools::exitFatal("Failed to create surface!", result);
	}

	delete[] pDisplays;
	delete[] pModeProperties;
	delete[] pDisplayProperties;
	delete[] pPlaneProperties;
}
#endif
