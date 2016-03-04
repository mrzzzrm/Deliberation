#include "ILUtils.h"

#include <iostream>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

PixelFormat ILFormatToPixelFormat(ILenum format)
{
    switch (format)
    {
    case IL_RGB:
        return PixelFormat_RGB_32_F;
    case IL_RGBA:
        return PixelFormat_RGBA_32_F;
    case IL_BGR:
        std::cout << "Warning!" << std::endl;
        return PixelFormat_RGB_32_F;
    case IL_BGRA:
        std::cout << "Warning!" << std::endl;
        return PixelFormat_RGBA_32_F;

    case IL_COLOUR_INDEX:    Fail("Can't load IL_COLOUR_INDEX"); break;
    case IL_ALPHA:           Fail("Can't load IL_ALPHA"); break;
    case IL_LUMINANCE:       Fail("Can't load IL_LUMINANCE"); break;
    case IL_LUMINANCE_ALPHA: Fail("Can't load IL_LUMINANCE_ALPHA"); break;

    default:
        Fail("Unknown IL format");
    }

    return PixelFormat_None;
}

}

