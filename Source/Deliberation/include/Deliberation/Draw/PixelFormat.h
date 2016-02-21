#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

enum PixelFormat
{
    PixelFormat_None = 0,

    PixelFormat_RGB_8U,
    PixelFormat_RGB_32U,

    PixelFormat_RGB_32F,
    PixelFormat_BGR_32F,
    PixelFormat_RGBA_32F,
    PixelFormat_BGRA_32F,

    PixelFormat_Depth_32F,

    _PixelFormat_Count_
};

enum PixelFormatType
{
    PixelFormatType_None = 0,
    PixelFormatType_Color,
    PixelFormatType_Depth,

    _PixelFormatType_Count_
};

unsigned int DELIBERATION_API PixelFormatChannelsPerPixel(PixelFormat format);
unsigned int DELIBERATION_API PixelFormatBytesPerPixel(PixelFormat format);
PixelFormatType DELIBERATION_API PixelFormatType(PixelFormat format);
gl::GLenum DELIBERATION_API PixelFormatToGLFormat(PixelFormat format);
gl::GLenum DELIBERATION_API PixelFormatToGLType(PixelFormat format);
gl::GLenum DELIBERATION_API PixelFormatToGLFragmentOutputType(PixelFormat format);
gl::GLenum DELIBERATION_API PixelFormatToGLSamplerType(PixelFormat format);
gl::GLenum DELIBERATION_API PixelFormatToGLInternalFormat(PixelFormat format);
const char * DELIBERATION_API PixelFormatToString(PixelFormat format);

}

