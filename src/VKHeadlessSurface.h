#ifndef _LIBVK_HEADLESS_SURFACE_H_
#define _LIBVK_HEADLESS_SURFACE_H_
#include "VKSurface.h"
#include <libcpp/libcpp.h>
EXTERN_C_BEGIN
namespaceBegin(foxintango)
class VKHeadlessSurface :public VKSurface{
protected:
    VKHeadlessSurface();
public:
    VKHeadlessSurface(uint32_t width,uint32_t height);
    
    virtual ~VKHeadlessSurface();
};
namespaceEnd
EXTERN_C_END
#endif
