#include <Deliberation/Draw/PixelFormat.h>

#include <glbinding/gl/enum.h>

namespace
{

struct PixelFormatMetaData
{
    const char * str;
    unsigned int channels;
    unsigned int bytesPerPixel;
    enum deliberation::PixelFormatType type;
    gl::GLenum glFormat;
    gl::GLenum glInternalFormat;
};

const PixelFormatMetaData META_DATA[deliberation::_PixelFormat_Count_] =
    {
        {"None",        0,  0,  deliberation::PixelFormatType_None,  gl::GL_NONE,             gl::GL_NONE},

        {"RGB_32F",     3,  12, deliberation::PixelFormatType_Color, gl::GL_RGB,              gl::GL_RGB},
        {"BGR_32F",     3,  12, deliberation::PixelFormatType_Color, gl::GL_BGR,              gl::GL_RGB},
        {"RGBA_32F",    4,  16, deliberation::PixelFormatType_Color, gl::GL_RGBA,             gl::GL_RGBA},
        {"BGRA_32F",    4,  16, deliberation::PixelFormatType_Color, gl::GL_BGRA,             gl::GL_RGBA},

        {"Depth_32F",   1,  4,  deliberation::PixelFormatType_Depth, gl::GL_DEPTH_COMPONENT,  gl::GL_DEPTH_COMPONENT32F},
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

enum PixelFormatType PixelFormatType(PixelFormat format)
{
    return META_DATA[format].type;
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

