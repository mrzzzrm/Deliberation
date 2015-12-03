#include "ILUtils.h"

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

PixelFormat ILFormatToPixelFormat(ILenum format)
{
    switch (format)
    {
    case IL_RGB:
        return PixelFormat_RGB_32F;
    case IL_RGBA:
        return PixelFormat_RGBA_32F;
    case IL_BGR:
        return PixelFormat_BGR_32F;
    case IL_BGRA:
        return PixelFormat_BGRA_32F;

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

