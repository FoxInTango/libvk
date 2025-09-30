#ifndef _LIBVK_DISPLAY_H_
#define _LIBVK_DISPLAY_H_
#include <libcpp/libcpp.h>
#include <vulkan/vulkan.h>
#include <string>
EXTERN_C_BEGIN
namespaceBegin(foxintango)

/** EDID 
 *     kms-cube kms-vulkan ksmroots
 * */
enum VKDisplayType{
    Physical,
    Wayland,
    XServer
};

class foxintangoAPI VKDisplay{
friend class VKContext;
friend class VKDisplaySurface;
protected:
    std::string deviceName;
    std::string displayName;
    VKDisplayType type;
union {
    struct {
        VkDisplayPropertiesKHR       displayProperties;
	uint32_t                     modePropertyCount;
        VkDisplayModePropertiesKHR*  modeProperties;
    }physical;
    struct {
        std::string name;
    }wayland;
    struct {
        std::string name;
    }xcb;
};
public:
    VKDisplay();
    VKDisplay(enum VKDisplayType type);
    ~VKDisplay();

};
namespaceEnd
EXTERN_C_END
#endif
