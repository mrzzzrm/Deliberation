#include <Deliberation/Draw/PixelFormat.h>

#include <glbinding/gl/enum.h>

namespace
{

struct PixelFormatMetaData
{
    const char * str;
    unsigned int channels;
    unsigned int bytesPerPixel;
    gl::GLenum glFormat;
    gl::GLenum glInternalFormat;
};

const PixelFormatMetaData META_DATA[deliberation::_PixelFormat_Count_] =
    {
        {"None",        0,  0,  gl::GL_NONE, gl::GL_NONE},

        {"RGB_32F",     3,  12, gl::GL_RGB,  gl::GL_RGB},
        {"BGR_32F",     3,  12, gl::GL_BGR,  gl::GL_RGB},
        {"RGBA_32F",    4,  16, gl::GL_RGBA, gl::GL_RGBA},
        {"BGRA_32F",    4,  16, gl::GL_BGRA, gl::GL_RGBA},

        {"Depth_32F",   1,  4,  gl::GL_RED,  gl::GL_DEPTH_COMPONENT32F},
    };
}

namespace deliberation
{

unsigned int PixelFormatChannelsPerPixel(PixelFormat format)
{
    return META_DATA[format].channels;
}

unsigned int PixelFormatBytesPerPixel(PixelFormat format)
{
    return META_DATA[format].bytesPerPixel;
}

gl::GLenum PixelFormatToGLFormat(PixelFormat format)
{
    return META_DATA[format].glFormat;
}

gl::GLenum PixelFormatToGLInternalFormat(PixelFormat format)
{
    return META_DATA[format].glInternalFormat;
}

const char * PixelFormatToString(PixelFormat format)
{
    return META_DATA[format].str;
}

}

