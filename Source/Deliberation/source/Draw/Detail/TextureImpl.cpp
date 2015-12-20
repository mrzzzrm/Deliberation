#include "TextureImpl.h"

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

namespace detail
{

std::shared_ptr<TextureImpl> TextureImpl::build(Context & context, unsigned int numFaces)
{
    auto impl = std::make_shared<TextureImpl>(context, numFaces);

    for (auto f = 0u; f < numFaces; f++)
    {
        impl->surfaces.push_back({impl, f});
    }

    return impl;
}

TextureImpl::TextureImpl(Context & context, unsigned int numFaces):
    context(context),
    glName(0u),
    width(0u),
    height(0u),
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

