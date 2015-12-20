#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

enum PixelFormat
{
    PixelFormat_None,
    PixelFormat_RGB_32F,
    PixelFormat_BGR_32F,
    PixelFormat_RGBA_32F,
    PixelFormat_BGRA_32F,
    _PixelFormat_Count_
};

unsigned int DELIBERATION_API PixelFormatChannelsPerPixel(PixelFormat format);
unsigned int DELIBERATION_API PixelFormatBytesPerPixel(PixelFormat format);
gl::GLenum DELIBERATION_API PixelFormatToGLFormat(PixelFormat format);
gl::GLenum DELIBERATION_API PixelFormatToGLInternalFormat(PixelFormat format);
const char * DELIBERATION_API PixelFormatToString(PixelFormat format);

}

