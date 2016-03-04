#include <Deliberation/Draw/PixelFormat.h>

#include <glbinding/gl/enum.h>

using namespace gl;

namespace deliberation
{

namespace detail
{

struct PixelFormatDesc
{
    PixelFormatEnum index;
    std::string     str;
    unsigned int    channels;
    unsigned int    bpp;
    PixelStorage    storage;
    PixelBitdepth   bitdepth;
    PixelType       type;
};

struct PixelFormatDescGL
{
    GLenum format;
    GLenum type;
    GLenum samplerValueType;
    GLenum fragmentOutputType;
    GLenum internalFormat;
};

}

}

using namespace deliberation;

namespace
{

const deliberation::detail::PixelFormatDesc DESCS[deliberation::_PixelFormat_Count_] =
    {
    //   Value                              String                  componentsPP     bytesPP     PixelStorage            PixelBitdepth          PixelType
        {PixelFormat_None,                  "None",                 0,               0,          PixelStorage_None,      PixelBitdepth_None,    PixelType_None},

        {PixelFormat_R_8_U,                 "R_8_U",                1,               1,          PixelStorage_U,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_R_16_U,                "R_16_U",               1,               2,          PixelStorage_U,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_R_32_U,                "R_32_U",               1,               4,          PixelStorage_U,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_R_8_I,                 "R_8_I",                1,               4,          PixelStorage_I,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_R_16_I,                "R_16_I",               1,               2,          PixelStorage_I,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_R_32_I,                "R_32_I",               1,               4,          PixelStorage_I,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_R_32_F,                "R_32_F",               1,               4,          PixelStorage_F,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_R_8_UN,                "R_8_UN",               1,               1,          PixelStorage_UN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_R_8_SN,                "R_8_SN",               1,               1,          PixelStorage_SN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_R_16_UN,               "R_16_UN",              1,               2,          PixelStorage_UN,        PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_R_16_SN,               "R_16_SN",              1,               2,          PixelStorage_SN,        PixelBitdepth_16,       PixelType_Color},

        {PixelFormat_RG_8_U,                "RG_8_U",               2,               2,          PixelStorage_U,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RG_16_U,               "RG_16_U",              2,               4,          PixelStorage_U,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RG_32_U,               "RG_32_U",              2,               8,          PixelStorage_U,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RG_8_I,                "RG_8_I",               2,               2,          PixelStorage_I,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RG_16_I,               "RG_16_I",              2,               4,          PixelStorage_I,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RG_32_I,               "RG_32_I",              2,               8,          PixelStorage_I,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RG_32_F,               "RG_32_F",              2,               8,          PixelStorage_F,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RG_8_UN,               "RG_8_UN",              2,               2,          PixelStorage_UN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RG_8_SN,               "RG_8_SN",              2,               2,          PixelStorage_SN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RG_16_UN,              "RG_16_UN",             2,               4,          PixelStorage_UN,        PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RG_16_SN,              "RG_16_SN",             2,               4,          PixelStorage_SN,        PixelBitdepth_16,       PixelType_Color},

        {PixelFormat_RGB_8_U,               "RGB_8_U",              3,               3,          PixelStorage_U,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGB_16_U,              "RGB_16_U",             3,               6,          PixelStorage_U,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RGB_32_U,              "RGB_32_U",             3,               12,         PixelStorage_U,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RGB_8_I,               "RGB_8_I",              3,               3,          PixelStorage_I,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGB_16_I,              "RGB_16_I",             3,               6,          PixelStorage_I,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RGB_32_I,              "RGB_32_I",             3,               12,         PixelStorage_I,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RGB_32_F,              "RGB_32_F",             3,               12,         PixelStorage_F,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RGB_8_UN,              "RGB_8_UN",             3,               3,          PixelStorage_UN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGB_8_SN,              "RGB_8_SN",             3,               3,          PixelStorage_SN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGB_16_UN,             "RGB_16_UN",            3,               6,          PixelStorage_UN,        PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RGB_16_SN,             "RGB_16_SN",            3,               6,          PixelStorage_SN,        PixelBitdepth_16,       PixelType_Color},

        {PixelFormat_RGBA_8_U,              "RGBA_8_U",             4,               4,          PixelStorage_U,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGBA_16_U,             "RGBA_16_U",            4,               8,          PixelStorage_U,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RGBA_32_U,             "RGBA_32_U",            4,               16,         PixelStorage_U,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RGBA_8_I,              "RGBA_8_I",             4,               4,          PixelStorage_I,         PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGBA_16_I,             "RGBA_16_I",            4,               8,          PixelStorage_I,         PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RGBA_32_I,             "RGBA_32_I",            4,               16,         PixelStorage_I,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RGBA_32_F,             "RGBA_32_F",            4,               16,         PixelStorage_F,         PixelBitdepth_32,       PixelType_Color},
        {PixelFormat_RGBA_8_UN,             "RGBA_8_UN",            4,               4,          PixelStorage_UN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGBA_8_SN,             "RGBA_8_SN",            4,               4,          PixelStorage_SN,        PixelBitdepth_8,        PixelType_Color},
        {PixelFormat_RGBA_16_UN,            "RGBA_16_UN",           4,               8,          PixelStorage_UN,        PixelBitdepth_16,       PixelType_Color},
        {PixelFormat_RGBA_16_SN,            "RGBA_16_SN",           4,               8,          PixelStorage_SN,        PixelBitdepth_16,       PixelType_Color},

        {PixelFormat_RGB_10_UN_A_2_UN,      "RGB_10_UN_A_2_UN",     4,               4,          PixelStorage_UN,        PixelBitdepth_Other,    PixelType_Color},
        {PixelFormat_RGB_10_U_A_2_U,        "RGB_10_U_A_2_U",       4,               4,          PixelStorage_U,         PixelBitdepth_Other,    PixelType_Color},
        {PixelFormat_R_11_F_G_11_F_B_10_F,  "R_11_F_G_11_F_B_10_F", 3,               4,          PixelStorage_F,         PixelBitdepth_Other,    PixelType_Color},

        {PixelFormat_Depth_16_UN,           "Depth_16_UN",          1,               2,          PixelStorage_UN,        PixelBitdepth_8,        PixelType_Depth},
        {PixelFormat_Depth_24_UN,           "Depth_24_UN",          1,               3,          PixelStorage_UN,        PixelBitdepth_24,       PixelType_Depth},
        {PixelFormat_Depth_32_F,            "Depth_32_F",           1,               4,          PixelStorage_F,         PixelBitdepth_32,       PixelType_Depth},

        {PixelFormat_Depth_24_UN_STENCIL,   "Depth_24_UN_STENCIL",  2,               4,          PixelStorage_UN,        PixelBitdepth_24,       PixelType_Depth},
        {PixelFormat_Depth_32_F_STENCIL,    "Depth_32_F_STENCIL",   2,               5,          PixelStorage_F,         PixelBitdepth_32,       PixelType_Depth},
    };

const deliberation::detail::PixelFormatDescGL DESCS_GL[deliberation::_PixelFormat_Count_] =
    {
      /*         name                           format              type                  samplerValueType   fragmentOutputType      internalFormat */
      /* PixelFormat_None */                {   GL_NONE,            GL_NONE,              GL_NONE,           GL_NONE,                GL_NONE          },

      /* PixelFormat_R_8_U */               {   GL_RED_INTEGER,     GL_UNSIGNED_BYTE,     GL_UNSIGNED_INT,   GL_UNSIGNED_INT,        GL_R8UI          },
      /* PixelFormat_R_16_U */              {   GL_RED_INTEGER,     GL_UNSIGNED_SHORT,    GL_UNSIGNED_INT,   GL_UNSIGNED_INT,        GL_R16UI         },
      /* PixelFormat_R_32_U */              {   GL_RED_INTEGER,     GL_UNSIGNED_INT,      GL_UNSIGNED_INT,   GL_UNSIGNED_INT,        GL_R32UI         },
      /* PixelFormat_R_8_I */               {   GL_RED_INTEGER,     GL_BYTE,              GL_INT,            GL_INT,                 GL_R8I           },
      /* PixelFormat_R_16_I */              {   GL_RED_INTEGER,     GL_SHORT,             GL_INT,            GL_INT,                 GL_R16I          },
      /* PixelFormat_R_32_I */              {   GL_RED_INTEGER,     GL_INT,               GL_INT,            GL_INT,                 GL_R32I          },
      /* PixelFormat_R_32_F */              {   GL_RED,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_R32F          },
      /* PixelFormat_R_8_UN */              {   GL_RED,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_R8            },
      /* PixelFormat_R_8_SN */              {   GL_RED,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_R8_SNORM      },
      /* PixelFormat_R_16_UN */             {   GL_RED,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_R16           },
      /* PixelFormat_R_16_SN */             {   GL_RED,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_R16_SNORM     },

      /* PixelFormat_RG_8_U */              {   GL_RG_INTEGER,      GL_UNSIGNED_BYTE,     GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC2,   GL_RG8UI         },
      /* PixelFormat_RG_16_U */             {   GL_RG_INTEGER,      GL_UNSIGNED_SHORT,    GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC2,   GL_RG16UI        },
      /* PixelFormat_RG_32_U */             {   GL_RG_INTEGER,      GL_UNSIGNED_INT,      GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC2,   GL_RG32UI        },
      /* PixelFormat_RG_8_I */              {   GL_RG_INTEGER,      GL_BYTE,              GL_INT,            GL_INT_VEC2,            GL_RG8I          },
      /* PixelFormat_RG_16_I */             {   GL_RG_INTEGER,      GL_SHORT,             GL_INT,            GL_INT_VEC2,            GL_RG16I         },
      /* PixelFormat_RG_32_I */             {   GL_RG_INTEGER,      GL_INT,               GL_INT,            GL_INT_VEC2,            GL_RG32I         },
      /* PixelFormat_RG_32_F */             {   GL_RG,              GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC2,          GL_RG32F         },
      /* PixelFormat_RG_8_UN */             {   GL_RG,              GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC2,          GL_RG8           },
      /* PixelFormat_RG_8_SN */             {   GL_RG,              GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC2,          GL_RG8_SNORM     },
      /* PixelFormat_RG_16_UN */            {   GL_RG,              GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC2,          GL_RG16          },
      /* PixelFormat_RG_16_SN */            {   GL_RG,              GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC2,          GL_RG16_SNORM    },

      /* PixelFormat_RGB_8_U */             {   GL_RGB_INTEGER,     GL_UNSIGNED_BYTE,     GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC3,   GL_RGB8UI        },
      /* PixelFormat_RGB_16_U */            {   GL_RGB_INTEGER,     GL_UNSIGNED_SHORT,    GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC3,   GL_RGB16UI       },
      /* PixelFormat_RGB_32_U */            {   GL_RGB_INTEGER,     GL_UNSIGNED_INT,      GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC3,   GL_RGB32UI       },
      /* PixelFormat_RGB_8_I */             {   GL_RGB_INTEGER,     GL_BYTE,              GL_INT,            GL_INT_VEC3,            GL_RGB8I         },
      /* PixelFormat_RGB_16_I */            {   GL_RGB_INTEGER,     GL_SHORT,             GL_INT,            GL_INT_VEC3,            GL_RGB16I        },
      /* PixelFormat_RGB_32_I */            {   GL_RGB_INTEGER,     GL_INT,               GL_INT,            GL_INT_VEC3,            GL_RGB32I        },
      /* PixelFormat_RGB_32_F */            {   GL_RGB,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC3,          GL_RGB32F        },
      /* PixelFormat_RGB_8_UN */            {   GL_RGB,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC3,          GL_RGB8          },
      /* PixelFormat_RGB_8_SN */            {   GL_RGB,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC3,          GL_RGB8_SNORM    },
      /* PixelFormat_RGB_16_UN */           {   GL_RGB,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC3,          GL_RGB16         },
      /* PixelFormat_RGB_16_SN */           {   GL_RGB,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC3,          GL_RGB16_SNORM   },

      /* PixelFormat_RGBA_8_U */            {   GL_RGBA_INTEGER,    GL_UNSIGNED_BYTE,     GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC4,   GL_RGBA8UI       },
      /* PixelFormat_RGBA_16_U */           {   GL_RGBA_INTEGER,    GL_UNSIGNED_SHORT,    GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC4,   GL_RGBA16UI      },
      /* PixelFormat_RGBA_32_U */           {   GL_RGBA_INTEGER,    GL_UNSIGNED_INT,      GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC4,   GL_RGBA32UI      },
      /* PixelFormat_RGBA_8_I */            {   GL_RGBA_INTEGER,    GL_BYTE,              GL_INT,            GL_INT_VEC4,            GL_RGBA8I        },
      /* PixelFormat_RGBA_16_I */           {   GL_RGBA_INTEGER,    GL_SHORT,             GL_INT,            GL_INT_VEC4,            GL_RGBA16I       },
      /* PixelFormat_RGBA_32_I */           {   GL_RGBA_INTEGER,    GL_INT,               GL_INT,            GL_INT_VEC4,            GL_RGBA32I       },
      /* PixelFormat_RGBA_32_F */           {   GL_RGBA,            GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC4,          GL_RGBA32F       },
      /* PixelFormat_RGBA_8_UN */           {   GL_RGBA,            GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC4,          GL_RGBA8         },
      /* PixelFormat_RGBA_8_SN */           {   GL_RGBA,            GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC4,          GL_RGBA8_SNORM   },
      /* PixelFormat_RGBA_16_UN */          {   GL_RGBA,            GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC4,          GL_RGBA16        },
      /* PixelFormat_RGBA_16_SN */          {   GL_RGBA,            GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC4,          GL_RGBA16_SNORM  },

      /* PixelFormat_RGB_10_UN_A_2_UN */    {   GL_RGBA,            GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC4,          GL_RGB10_A2      },
      /* PixelFormat_RGB_10_U_A_2_U */      {   GL_RGBA_INTEGER,    GL_UNSIGNED_INT,      GL_UNSIGNED_INT,   GL_UNSIGNED_INT_VEC4,   GL_RGB10_A2UI    },
      /* PixelFormat_R_11_F_G_11_F_B_10_F */{   GL_RGB,             GL_FLOAT,             GL_FLOAT,          GL_FLOAT_VEC3,          GL_R11F_G11F_B10F},

      /* PixelFormat_Depth_16_UN */         {   GL_DEPTH_COMPONENT, GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_DEPTH_COMPONENT16},
      /* PixelFormat_Depth_24_UN */         {   GL_DEPTH_COMPONENT, GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_DEPTH_COMPONENT24},
      /* PixelFormat_Depth_32_F */          {   GL_DEPTH_COMPONENT, GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_DEPTH_COMPONENT32F},

      /* PixelFormat_Depth_24_UN_STENCIL */ {   GL_DEPTH_COMPONENT, GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_DEPTH24_STENCIL8},
      /* PixelFormat_Depth_32_F_STENCIL */  {   GL_DEPTH_COMPONENT, GL_FLOAT,             GL_FLOAT,          GL_FLOAT,               GL_DEPTH32F_STENCIL8},
    };
}

namespace deliberation
{

PixelFormat::PixelFormat():
    m_desc(&DESCS[PixelFormat_None])
{

}

PixelFormat::PixelFormat(PixelFormatEnum format):
    m_desc(&DESCS[format])
{

}

PixelFormatEnum PixelFormat::value() const
{
    return m_desc->index;
}

unsigned int PixelFormat::componentsPerPixel() const
{
    return m_desc->channels;
}

unsigned int PixelFormat::bytesPerPixel() const
{
    return m_desc->bpp;
}

PixelType PixelFormat::pixelType() const
{
    return m_desc->type;
}

const std::string & PixelFormat::toString() const
{
    return m_desc->str;
}

bool PixelFormat::operator==(const PixelFormat & format) const
{
    return m_desc == format.m_desc;
}

bool PixelFormat::operator!=(const PixelFormat & format) const
{
    return m_desc != format.m_desc;
}

gl::GLenum PixelFormat::glFormat() const
{
    return DESCS_GL[m_desc->index].format;
}

gl::GLenum PixelFormat::glType() const
{
    return DESCS_GL[m_desc->index].type;
}

gl::GLenum PixelFormat::glFragmentOutputType() const
{
    return DESCS_GL[m_desc->index].fragmentOutputType;
}

gl::GLenum PixelFormat::glSamplerValueType() const
{
    return DESCS_GL[m_desc->index].samplerValueType;
}

gl::GLenum PixelFormat::glInternalFormat() const
{
    return DESCS_GL[m_desc->index].internalFormat;
}

}

