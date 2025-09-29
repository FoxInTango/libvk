#include "VKContext.h"
using namespace foxintango;
#include "VKDevice.h"
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <unistd.h>
std::vector<const char*> supportedDeviceExtensions;
std::vector<const char*> supportedInstanceExtensions;
std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

// Physical device (GPU) that Vulkan will use
// VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };

/** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
VkPhysicalDeviceFeatures enabledFeatures{};
/** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
std::vector<const char*> enabledDeviceExtensions;
/** @brief Set of instance extensions to be enabled for this example (must be set in the derived constructor) */
std::vector<const char*> enabledInstanceExtensions;
/** @brief Set of layer settings to be enabled for this example (must be set in the derived constructor) */
//std::vector<VkLayerSettingEXT> enabledLayerSettings;
/** @brief Optional pNext structure for passing extension structures to device creation */
void* deviceCreatepNextChain = nullptr;
/** @brief Logical device, application's view of the physical device (GPU) */
//VkDevice device{ VK_NULL_HANDLE };
// Handle to the device graphics queue that command buffers are submitted to
VkQueue queue{ VK_NULL_HANDLE };
// Depth buffer format (selected during Vulkan initialization)
VkFormat depthFormat{VK_FORMAT_UNDEFINED};
// Command buffer pool
//VkCommandPool cmdPool{ VK_NULL_HANDLE };
/** @brief Pipeline stages used to wait at for graphics queue submissions */
VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// Contains command buffers and semaphores to be presented to the queue
VkSubmitInfo submitInfo{};
// Command buffers used for rendering
//std::vector<VkCommandBuffer> drawCmdBuffers;
// Global render pass for frame buffer writes
//VkRenderPass renderPass{ VK_NULL_HANDLE };
// List of available frame buffers (same as number of swap chain images)
//std::vector<VkFramebuffer>frameBuffers;
// Active frame buffer index
//uint32_t currentBuffer = 0;
// Descriptor set pool
//VkDescriptorPool descriptorPool{ VK_NULL_HANDLE };
// List of shader modules created (stored for cleanup)
//std::vector<VkShaderModule> shaderModules;
// Pipeline cache object
//VkPipelineCache pipelineCache{ VK_NULL_HANDLE };
// Wraps the swap chain to present images (framebuffers) to the windowing system
// VulkanSwapChain swapChain;
// Synchronization semaphores
struct {
    // Swap chain image presentation
    VkSemaphore presentComplete;
    // Command buffer submission and execution
    VkSemaphore renderComplete;
} semaphores{};
std::vector<VkFence> waitFences;
bool requiresStencil{ false };

/** What are missed here than the vulkan example base 
 *  1,Debug
 *  2,Mac
 * */
/** Physical GPU Devices
 *  Physical GPU Device Properties
 *  Physical Displays
 *  Physical Display Properties
 *  Physical Display Modes
 * */
std::vector<std::string>                               physicalDeviceArray;
std::map<std::string,VkPhysicalDevice>                 physicalDeviceMap;
std::map<std::string,VkPhysicalDeviceProperties>       physicalDevicePropertiesMap;
std::map<std::string,VkPhysicalDeviceFeatures>         physicalDeviceFeaturesMap;
std::map<std::string,VkPhysicalDeviceMemoryProperties> physicalDeviceMemoryPropertiesMap;

VKContext::VKContext(){
    #ifdef PLATFORM_LINUX
    instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
    instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
    //instanceExtensions.push_back(VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME);
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
    appInfo.apiVersion       = VK_API_VERSION_1_3;//apiVersion;

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if (!instanceExtensions.empty()) {
        instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
    }
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

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &vulkan);
    if(result != VK_SUCCESS){
        std::cerr << "VKInstance Creating Failed.";
    }
    printf("VkInstance is Ready.\n");     
}
/**
 // GPU selection

    // Select physical device to be used for the Vulkan example
    // Defaults to the first device unless specified by command line
    uint32_t selectedDevice = 1;
    // TODO
    // Enable Features 
    // Enable Extensions
    
    VkPhysicalDevice physicalDevice = physicalDevices[selectedDevice];
    VKDevice* device = new VKDevice(physicalDevice);
    devices.push_back(device);
    deviceMap.insert(std::pair<std::string,uint32_t>("default",devices.size() - 1));
    //device.bind(physicalDevice);
    result = device->createLogicalDevice(enabledFeatures,enabledDeviceExtensions,deviceCreatepNextChain,false,VK_QUEUE_GRAPHICS_BIT);
    if(result != VK_SUCCESS){
        vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(result), result);
        return;// false;
    }
    //device.logicalDevice;

    // Get a graphics queue from the device
    vkGetDeviceQueue(device->logicalDevice,device.queueFamilyIndices.graphics, 0, &queue);

    // Find a suitable depth and/or stencil format
    VkBool32 validFormat{ false };
    // Samples that make use of stencil will require a depth + stencil format, so we select from a different list
    if(requiresStencil) {
        validFormat = vks::tools::getSupportedDepthStencilFormat(device.physicalDevice, &depthFormat);
    } else {
        validFormat = vks::tools::getSupportedDepthFormat(physicalDevice, &depthFormat);
    }
    assert(validFormat);

    //swapChain.setContext(instance, physicalDevice, device);

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = vks::initializers::semaphoreCreateInfo();
    // Create a semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queue
    VK_CHECK_RESULT(vkCreateSemaphore(device.logicalDevice, &semaphoreCreateInfo, nullptr, &semaphores.presentComplete));
    // Create a semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been submitted and executed
    VK_CHECK_RESULT(vkCreateSemaphore(device.logicalDevice, &semaphoreCreateInfo, nullptr, &semaphores.renderComplete));

    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    // Command buffer submission info is set by each example
    submitInfo = vks::initializers::submitInfo();
    submitInfo.pWaitDstStageMask = &submitPipelineStages;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &semaphores.presentComplete;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &semaphores.renderComplete;

    printf("Context Ready. device.logicalDevice:%p\n",device.logicalDevice);
 * */
VKContext::~VKContext(){
    //vkDestroyCommandPool(device, cmdPool, nullptr);
    //vkDestroySemaphore(device.logicalDevice, semaphores.presentComplete, nullptr);
    //vkDestroySemaphore(device.logicalDevice, semaphores.renderComplete, nullptr);
    /*
    for (auto& fence : waitFences) {
        vkDestroyFence(device, fence, nullptr);
    }
    */

    /*
    VkResult res = vkDeviceWaitIdle(device.logicalDevice);
    switch(res){
        case VK_ERROR_DEVICE_LOST:{printf("VK_ERROR_DEVICE_LOST\n");}break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:{printf("VK_ERROR_OUT_OF_DEVICE_MEMORY\n");}break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:{printf("VK_ERROR_OUT_OF_HOST_MEMORY\n");};break;
        case VK_ERROR_UNKNOWN:{printf("VK_ERROR_UNNOWN\n");}break;
        case VK_ERROR_VALIDATION_FAILED_EXT:{printf("VK_ERROR_VALIDATION_FAILED\n");}break;
        case VK_SUCCESS:{printf("vkDeviceWaitIdle OK\n");}break;
        default:break;
    }*/
    /** destroy all VKDevice Instances.
     * */
    clean();
    /*
    vkDestroyInstance(vkInstance, nullptr);
    */
}

void VKContext::clean(){
    destroyDevices();
}

void VKContext::enumeratePhysicalDevices(){
    // Physical device
    uint32_t gpuCount = 0;
    // Get number of available physical devices
    vkEnumeratePhysicalDevices(vulkan, &gpuCount, nullptr);
    if (gpuCount == 0) {
        printf("No GPU Device Found.\n");
        return;
    }
    printf("%d GPU Devices Found.\n",gpuCount);

    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    VkResult result = vkEnumeratePhysicalDevices(vulkan, &gpuCount, physicalDevices.data());
    if (result != VK_SUCCESS) {
        printf("Could not enumerate physical devices.\n");
        return;
    }

    /* Record Physical Devices
        std::map<std::string,VkPhysicalDevice>                 physicalDeviceMap;
        std::map<std::string,VkPhysicalDeviceProperties>       physicalDevicePropertiesMap;
        std::map<std::string,VkPhysicalDeviceFeatures>         physicalDeviceFeaturesMap;
        std::map<std::string,VkPhysicalDeviceMemoryProperties> physicalDeviceMemoryPropertiesMap;
*/
    for(std::vector<VkPhysicalDevice>::iterator iter = physicalDevices.begin();iter != physicalDevices.end();iter ++){
        struct VkPhysicalDeviceProperties deviceProperties;/** /usr/include/vulkan/vulkan_core.h */
        VkPhysicalDeviceFeatures deviceFeatures;
        VkPhysicalDeviceMemoryProperties memoryProperties;
        // Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
        vkGetPhysicalDeviceProperties(*iter, &deviceProperties);
        vkGetPhysicalDeviceFeatures(*iter, &deviceFeatures);
        vkGetPhysicalDeviceMemoryProperties(*iter, &memoryProperties);

	physicalDeviceArray.push_back(deviceProperties.deviceName);
        physicalDeviceMap.insert(std::pair<std::string,VkPhysicalDevice>(deviceProperties.deviceName,*iter));
	physicalDevicePropertiesMap.insert(std::pair<std::string,VkPhysicalDeviceProperties>(deviceProperties.deviceName,deviceProperties));
        physicalDeviceFeaturesMap.insert(std::pair<std::string,VkPhysicalDeviceFeatures>(deviceProperties.deviceName,deviceFeatures));
        physicalDeviceMemoryPropertiesMap.insert(std::pair<std::string,VkPhysicalDeviceMemoryProperties>(deviceProperties.deviceName,memoryProperties));
        /*
	printf("%s :\nAPI    Version:%u\nDriver Version:%u\nVendorID:%u\nDeviceID:%u\n",\
			deviceProperties.deviceName,             \
			deviceProperties.apiVersion,             \
			deviceProperties.driverVersion,          \
			deviceProperties.vendorID,               \
			deviceProperties.deviceID);
        */
        std::cout << "Device [" << iter - physicalDevices.begin() << "] : " << deviceProperties.deviceName << std::endl;
        std::cout << " Type   : " << vks::tools::physicalDeviceTypeString(deviceProperties.deviceType) << "\n";
        std::cout << " API    : " << (deviceProperties.apiVersion    >> 22) << "." << ((deviceProperties.apiVersion    >> 12) & 0x3ff) << "." << (deviceProperties.apiVersion    & 0xfff)    << "\n";
        std::cout << " Driver : " << (deviceProperties.driverVersion >> 22) << "." << ((deviceProperties.driverVersion >> 12) & 0x3ff) << "." << (deviceProperties.driverVersion & 0xfff) << "\n";
    }
}

uint32_t VKContext::physicalDeviceCount(){
    return physicalDeviceArray.size();
}

VkPhysicalDevice VKContext::physicalDeviceAt(uint32_t index){
    return index < physicalDeviceArray.size() && physicalDeviceMap.count(physicalDeviceArray[index]) ? physicalDeviceMap.at(physicalDeviceArray[index]) : VK_NULL_HANDLE;
}

VkPhysicalDevice VKContext::physicalDeviceAt(const char* name){
    return physicalDeviceMap.count(name) ? physicalDeviceMap.at(name) : VK_NULL_HANDLE;
}

VkPhysicalDeviceProperties* VKContext::physicalDevicePropertiesOf(uint32_t index){
    return index < physicalDeviceArray.size() && physicalDevicePropertiesMap.count(physicalDeviceArray[index]) ? &physicalDevicePropertiesMap.at(physicalDeviceArray[index]) : nullptr;
}

VkPhysicalDeviceProperties* VKContext::physicalDevicePropertiesOf(const char* name){
    return physicalDevicePropertiesMap.count(name) ? &physicalDevicePropertiesMap.at(name) : nullptr;
}

VkPhysicalDeviceFeatures* VKContext::physicalDeviceFeaturesOf(uint32_t index){
    return index < physicalDeviceArray.size() && physicalDeviceFeaturesMap.count(physicalDeviceArray[index]) ? &physicalDeviceFeaturesMap.at(physicalDeviceArray[index]) : nullptr;
}

VkPhysicalDeviceFeatures* VKContext::physicalDeviceFeaturesOf(const char* name){
    return physicalDeviceFeaturesMap.count(name) ? &physicalDeviceFeaturesMap.at(name) : nullptr;
}

VkPhysicalDeviceMemoryProperties* VKContext::physicalDeviceMemoryPropertiesOf(uint32_t index){
    return index < physicalDeviceArray.size() && physicalDeviceMemoryPropertiesMap.count(physicalDeviceArray[index]) ? &physicalDeviceMemoryPropertiesMap.at(physicalDeviceArray[index]) : nullptr;
}

VkPhysicalDeviceMemoryProperties* VKContext::physicalDeviceMemoryPropertiesOf(const char* name){
    return physicalDeviceMemoryPropertiesMap.count(name) ? &physicalDeviceMemoryPropertiesMap.at(name) : nullptr;
}
/*
void VKContext::enumerateDisplay(){

    uint32_t displayPropertyCount;

    for(std::map<std::string,VkPhysicalDevice>::iterator iter = physicalDeviceMap.begin();iter != physicalDeviceMap.end();iter ++) {
    std::cout << "GPUDevice: " << iter->first << std::endl;
    // Get display property
    VkPhysicalDevice physicalDevice = iter->second;
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
         
    for(uint32_t i = 0; i < displayPropertyCount;++i){
        std::cout << "Display " << pDisplayProperties[i].displayName << ":" << std::endl;
        display = pDisplayProperties[i].display;
        uint32_t modeCount;
        vkGetDisplayModePropertiesKHR(physicalDevice, display, &modeCount, NULL);
        pModeProperties = new VkDisplayModePropertiesKHR[modeCount];
        vkGetDisplayModePropertiesKHR(physicalDevice, display, &modeCount, pModeProperties);
        if( 1 > modeCount) {
             printf("no mode found for display %d with modeCount: %u\n",i,modeCount);
        } else printf("modeCount : %u \n",modeCount);

        for(uint32_t j = 0; j < modeCount; ++j){
            const VkDisplayModePropertiesKHR* mode = &pModeProperties[j];
            // to be delete : sparrow
            printf("mode %d width: %d height: %d\n",j,mode->parameters.visibleRegion.width,mode->parameters.visibleRegion.height);
            
	    if(mode->parameters.visibleRegion.width == width && mode->parameters.visibleRegion.height == height){
                displayMode = mode->displayMode;
                foundMode = true;
                break;
            }
            
        }
        if(foundMode){ break;}
        delete [] pModeProperties;
    }
    } 
}
*/
VKDevice* VKContext::createDevice(VkPhysicalDevice           physicalDevice,
                                  VkPhysicalDeviceFeatures   enabledFeatures,
                                  std::vector<const char *>  enabledDeviceExtensions,
                                  void*                      pNextChain,
                                  bool                       useSwapChain,
                                  VkQueueFlags               requestedQueueTypes,
                                  const char*                name){
    VKDevice* device = new VKDevice(physicalDevice);
    if(device) {
        VkResult result = device->createLogicalDevice(enabledFeatures,enabledDeviceExtensions,pNextChain,useSwapChain,requestedQueueTypes);
        if(result != VK_SUCCESS){
            vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(result), result);
        }

        std::string deviceName = name ? name : std::string("VKDevice-") + std::to_string(deviceMap.size());
	device->name = deviceName;
	deviceMap.insert(std::pair<std::string,VKDevice*>(deviceName,device));
    }

    return device;
}

VKDevice* VKContext::createDevice(const char*                physicalDeviceName,
                                  VkPhysicalDeviceFeatures   enabledFeatures,
                                  std::vector<const char *>  enabledDeviceExtensions,
                                  void*                      pNextChain,
                                  bool                       useSwapChain,
                                  VkQueueFlags               requestedQueueTypes,
                                  const char*                name){
    return createDevice(physicalDeviceAt(physicalDeviceName),enabledFeatures,enabledDeviceExtensions,pNextChain,useSwapChain,requestedQueueTypes,name);
}

void VKContext::destroyDevice(VKDevice*   device){
    if(device) { 
        delete device;
        deviceMap.erase(device->name);
    }

}
void VKContext::destroyDevice(const char* name){
    if(deviceMap.count(name)){
        VKDevice* device = deviceMap.at(name);
	delete device;
	deviceMap.erase(name);
    }
}
void VKContext::destroyDevices(){
    for(std::map<std::string,VKDevice*>::iterator iter = deviceMap.begin();iter != deviceMap.end();iter ++){
        VKDevice* device = iter->second;
	delete device;
	deviceMap.erase(iter);
    }
}

VKDevice* VKContext::deviceAt(const char* name){
    return deviceMap.count(name) ? deviceMap.at(name) : nullptr;
}

void VKContext::enableInstanceExtensions(){}
void VKContext::enableDeviceFeatures(){}
void VKContext::enableDeviceExtensions(){}
