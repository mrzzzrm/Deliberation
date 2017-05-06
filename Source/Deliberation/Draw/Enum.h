#pragma once

#include <glbinding/gl/enum.h>

namespace deliberation
{

enum class TextureType: unsigned int
{
    Texture2d   = (unsigned int)gl::GL_TEXTURE_2D,
    CubeMap     = (unsigned int)gl::GL_TEXTURE_CUBE_MAP,
    None
};

}