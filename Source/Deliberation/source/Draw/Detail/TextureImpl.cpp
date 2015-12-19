#include "TextureImpl.h"

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

namespace detail
{

TextureImpl::TextureImpl(gl::GLuint glName,
                         unsigned int width,
                         unsigned int height,
                         unsigned int numFaces):
    glName(glName),
    width(width),
    height(height),
    numFaces(numFaces)
{
    baseLevel = 0;
    maxLevel = 0;
    minFilter = gl::GL_LINEAR;
    maxFilter = gl::GL_LINEAR;

    if (numFaces == 1)
    {
        type = gl::GL_TEXTURE_2D;
    }
    else
    {
        Assert(numFaces == 6, "");
        type = gl::GL_TEXTURE_CUBE_MAP;
    }
}

}

}

