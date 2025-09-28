#ifndef _LIBVK_DISPLAY_H_
#define _LIBVK_DISPLAY_H_
#include <libcpp/libcpp.h>
#include <string>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class foxintangoAPI VKDisplay{
protected:
    std::string deviceName;
    std::string displayName;
public:
    VKDisplay();
    ~VKDisplay();

};
namespaceEnd
EXTERN_C_END
#endif
