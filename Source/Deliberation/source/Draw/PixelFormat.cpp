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
    gl::GLenum glType;
    gl::GLenum glSamplerType;
    gl::GLenum glFragmentOutputType;
    gl::GLenum glInternalFormat;
};

const PixelFormatMetaData META_DATA[deliberation::_PixelFormat_Count_] =
    {
        {"None",        0,  0,  deliberation::PixelFormatType_None,  gl::GL_NONE,             gl::GL_NONE,              gl::GL_NONE,                         gl::GL_NONE,                gl::GL_NONE},

        {"RGB_8U",      3,  3,  deliberation::PixelFormatType_Color, gl::GL_RGB_INTEGER,      gl::GL_UNSIGNED_BYTE,     gl::GL_UNSIGNED_INT_SAMPLER_2D,      gl::GL_UNSIGNED_INT_VEC3,   gl::GL_RGB8UI},
        {"RGB_32U",     3,  12, deliberation::PixelFormatType_Color, gl::GL_RGB_INTEGER,      gl::GL_UNSIGNED_INT,      gl::GL_UNSIGNED_INT_SAMPLER_2D,      gl::GL_UNSIGNED_INT_VEC3,   gl::GL_RGB32UI},

        {"RGB_32F",     3,  12, deliberation::PixelFormatType_Color, gl::GL_RGB,              gl::GL_FLOAT,             gl::GL_SAMPLER_2D,                   gl::GL_FLOAT_VEC3,          gl::GL_RGB},
        {"BGR_32F",     3,  12, deliberation::PixelFormatType_Color, gl::GL_BGR,              gl::GL_FLOAT,             gl::GL_SAMPLER_2D,                   gl::GL_FLOAT_VEC3,          gl::GL_RGB},
        {"RGBA_32F",    4,  16, deliberation::PixelFormatType_Color, gl::GL_RGBA,             gl::GL_FLOAT,             gl::GL_SAMPLER_2D,                   gl::GL_FLOAT_VEC4,          gl::GL_RGBA},
        {"BGRA_32F",    4,  16, deliberation::PixelFormatType_Color, gl::GL_BGRA,             gl::GL_FLOAT,             gl::GL_SAMPLER_2D,                   gl::GL_FLOAT_VEC4,          gl::GL_RGBA},

        {"Depth_32F",   1,  4,  deliberation::PixelFormatType_Depth, gl::GL_DEPTH_COMPONENT,  gl::GL_FLOAT,             gl::GL_SAMPLER_2D,                   gl::GL_NONE,                gl::GL_DEPTH_COMPONENT32F},
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

gl::GLenum PixelFormatToGLType(PixelFormat format)
{
    return META_DATA[format].glType;
}

gl::GLenum PixelFormatToGLInternalFormat(PixelFormat format)
{
    return META_DATA[format].glInternalFormat;
}

gl::GLenum PixelFormatToGLFragmentOutputType(PixelFormat format)
{
    return META_DATA[format].glFragmentOutputType;
}

gl::GLenum PixelFormatToGLSamplerType(PixelFormat format)
{
    return META_DATA[format].glSamplerType;
}

const char * PixelFormatToString(PixelFormat format)
{
    return META_DATA[format].str;
}

}

