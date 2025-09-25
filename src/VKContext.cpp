#include "VKContext.h"
using namespace foxintango;
#include <iostream>
#include <string>
#include <vector>
#include <string.h>


std::vector<std::string> supportedInstanceExtensions;
std::vector<std::string> neededInstanceExtensions;

/** What are missed here than the vulkan example base 
 *  1,Debug
 *  2,Mac
 * */
VKContext::VKContext(){
    #ifdef PLATFORM_LINUX
    neededInstanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
    neededInstanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
    //neededInstanceExtensions.push_back(VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME);
    #endif

    // Get extensions supported by the instance and store for later use
    uint32_t extCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
    if(extCount > 0) {
        std::vector<VkExtensionProperties> extensions(extCount);
        if(vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS){
            for(VkExtensionProperties& extension : extensions){
                supportedInstanceExtensions.push_back(extension.extensionName);
            }
        }
    }
    /** NOTICE
     *  InstanceExtensions needed on MacOS/iOS 
     *  See examplebase.cpp : enabledInstanceExtensions
     * */
    VkApplicationInfo appInfo{};
    appInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanApplication";
    appInfo.pEngineName      = "VulkanContext";
    appInfo.apiVersion       = VK_API_VERSION_1_0;//apiVersion;

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
    // Check if this layer is available at instance level
    uint32_t instanceLayerCount;
    vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
    std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
    vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
    bool validationLayerPresent = false;
    for (VkLayerProperties& layer : instanceLayerProperties) {
        if (strcmp(layer.layerName, validationLayerName) == 0) {
            validationLayerPresent = true;
            break;
        }
    }
    if (validationLayerPresent) {
        instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
        instanceCreateInfo.enabledLayerCount = 1;
    } else {
        std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
    }

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance);
    if(result != VK_SUCCESS){
        std::cerr << "VKInstance Creating Failed.";
    }
    printf("VKContext is Ready.\n");
    // Physical device
    uint32_t gpuCount = 0;
    // Get number of available physical devices
    vkEnumeratePhysicalDevices(vkInstance, &gpuCount, nullptr);
    if (gpuCount == 0) {
        return;//FALSE
    }
    printf("%d GPU Devices Found.\n",gpuCount);

    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    result = vkEnumeratePhysicalDevices(vkInstance, &gpuCount, physicalDevices.data());
    if (result != VK_SUCCESS) {
        printf("Could not enumerate physical devices : \n");
        return; //FALSE
    }

    for(std::vector<VkPhysicalDevice>::iterator iter = physicalDevices.begin();iter != physicalDevices.end();iter ++){
        struct VkPhysicalDeviceProperties deviceProperties;/** /usr/include/vulkan/vulkan_core.h */
        VkPhysicalDeviceFeatures deviceFeatures;
        VkPhysicalDeviceMemoryProperties memoryProperties;
        // Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
        vkGetPhysicalDeviceProperties(*iter, &deviceProperties);
        vkGetPhysicalDeviceFeatures(*iter, &deviceFeatures);
        vkGetPhysicalDeviceMemoryProperties(*iter, &memoryProperties);
        printf("%s :\nAPI    Version:%u\nDriver Version:%u\nVendorID:%u\nDeviceID:%u\n",\
			deviceProperties.deviceName,             \
			deviceProperties.apiVersion,             \
			deviceProperties.driverVersion,          \
			deviceProperties.vendorID,               \
			deviceProperties.deviceID);
    }
    // GPU selection

    // Select physical device to be used for the Vulkan example
    // Defaults to the first device unless specified by command line
    uint32_t selectedDevice = 0;
}
VKContext::~VKContext(){
// destroy vkInstance
}
