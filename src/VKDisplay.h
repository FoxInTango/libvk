#ifndef _LIBVK_DISPLAY_H_
#define _LIBVK_DISPLAY_H_
#include <libcpp/libcpp.h>
#include <vulkan/vulkan.h>
#include <string>
EXTERN_C_BEGIN
namespaceBegin(foxintango)

enum VKDisplayType{
    Physical,
    Wayland,
    XServer
};

class foxintangoAPI VKDisplay{
protected:
    std::string deviceName;
    std::string displayName;
    VKDisplayType type;
union {
    struct {
        VkDisplayPropertiesKHR* properties;
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
    ~VKDisplay();

};
namespaceEnd
EXTERN_C_END
#endif
