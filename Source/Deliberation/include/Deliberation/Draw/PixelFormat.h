#pragma once

#include <glbinding/gl/enum.h>

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

unsigned int PixelFormatChannelsPerPixel(PixelFormat format);
unsigned int PixelFormatBytesPerPixel(PixelFormat format);
gl::GLenum PixelFormatToGLFormat(PixelFormat format);
gl::GLenum PixelFormatToGLInternalFormat(PixelFormat format);
const char * PixelFormatToString(PixelFormat format);

}

