#pragma once

#include <string>

#include <glbinding/gl/enum.h>

namespace deliberation
{

enum PixelFormatEnum
{
    PixelFormat_None = 0,

    /*
        1 Channel
    */
    PixelFormat_R_8_U,
    PixelFormat_R_16_U,
    PixelFormat_R_32_U,

    PixelFormat_R_8_I,
    PixelFormat_R_16_I,
    PixelFormat_R_32_I,

    PixelFormat_R_32_F,

    PixelFormat_R_8_UN,
    PixelFormat_R_8_SN,

    PixelFormat_R_16_UN,
    PixelFormat_R_16_SN,

    /*
        2 Channel
    */
    PixelFormat_RG_8_U,
    PixelFormat_RG_16_U,
    PixelFormat_RG_32_U,

    PixelFormat_RG_8_I,
    PixelFormat_RG_16_I,
    PixelFormat_RG_32_I,

    PixelFormat_RG_32_F,

    PixelFormat_RG_8_UN,
    PixelFormat_RG_8_SN,

    PixelFormat_RG_16_UN,
    PixelFormat_RG_16_SN,

    /*
        3 Channel
    */
    PixelFormat_RGB_8_U,
    PixelFormat_RGB_16_U,
    PixelFormat_RGB_32_U,

    PixelFormat_RGB_8_I,
    PixelFormat_RGB_16_I,
    PixelFormat_RGB_32_I,

    PixelFormat_RGB_32_F,

    PixelFormat_RGB_8_UN,
    PixelFormat_RGB_8_SN,

    PixelFormat_RGB_16_UN,
    PixelFormat_RGB_16_SN,

    /*
        4 Channel
    */
    PixelFormat_RGBA_8_U,
    PixelFormat_RGBA_16_U,
    PixelFormat_RGBA_32_U,

    PixelFormat_RGBA_8_I,
    PixelFormat_RGBA_16_I,
    PixelFormat_RGBA_32_I,

    PixelFormat_RGBA_32_F,

    PixelFormat_RGBA_8_UN,
    PixelFormat_RGBA_8_SN,

    PixelFormat_RGBA_16_UN,
    PixelFormat_RGBA_16_SN,

    /*
        Special
    */
    PixelFormat_RGB_10_UN_A_2_UN,
    PixelFormat_RGB_10_U_A_2_U,
    PixelFormat_R_11_F_G_11_F_B_10_F,

    /*
        Depth/Stencil
    */
    PixelFormat_Depth_16_UN,
    PixelFormat_Depth_24_UN,
    PixelFormat_Depth_32_F,

    PixelFormat_Depth_24_UN_STENCIL,
    PixelFormat_Depth_32_F_STENCIL,

    /*
        Count
    */
    _PixelFormat_Count_
};

enum PixelStorage
{
    PixelStorage_None,
    PixelStorage_F,
    PixelStorage_I,
    PixelStorage_U,
    PixelStorage_SN,
    PixelStorage_UN
};

enum PixelBitdepth
{
    PixelBitdepth_None,
    PixelBitdepth_8,
    PixelBitdepth_16,
    PixelBitdepth_24,
    PixelBitdepth_32,
    PixelBitdepth_Other
};

enum PixelType
{
    PixelType_None = 0,
    PixelType_Color,
    PixelType_Depth,

    _PixelType_Count_
};

namespace detail
{
    struct PixelFormatDesc;
}

class PixelFormat final
{
public:
    template<typename T>
    static constexpr PixelFormatEnum resolve();

public:
    PixelFormat();
    PixelFormat(PixelFormatEnum format);

    PixelFormatEnum value() const;

    unsigned int componentsPerPixel() const;
    unsigned int bytesPerPixel() const;

    PixelType pixelType() const;

    const std::string & toString() const;

    bool operator==(const PixelFormat & format) const;
    bool operator!=(const PixelFormat & format) const;

    /*
        GL specific stuff
        TODO
            Move this elsewhere. Out of sight.
    */
    gl::GLenum glFormat() const;
    gl::GLenum glType() const;
    gl::GLenum glFragmentOutputType() const;
    gl::GLenum glSamplerValueType() const;
    gl::GLenum glInternalFormat() const;

private:
    const detail::PixelFormatDesc *   m_desc;
};

}

#include <Deliberation/Draw/PixelFormat.inl>
