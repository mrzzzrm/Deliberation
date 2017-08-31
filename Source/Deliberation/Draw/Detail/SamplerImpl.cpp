#include "SamplerImpl.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Draw/Detail/TextureImpl.h>
#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{
SamplerImpl::SamplerImpl(
    DrawContext & drawContext,
    gl::GLenum    uniformType,
    gl::GLenum    valueType,
    gl::GLuint    location)
    : drawContext(drawContext)
    , type(uniformType)
    , valueType(valueType)
    , location(location)
{
    gl::glGenSamplers(1, &glName);
    AssertM(glName != 0, "Failed to create GL sampler object");

    wrap[0] = TextureWrap::Repeat;
    wrap[1] = TextureWrap::Repeat;
    wrap[2] = TextureWrap::Repeat;

    minFilter = TextureFilter::Nearest;
    magFilter = TextureFilter::Nearest;
}

SamplerImpl::~SamplerImpl()
{
    drawContext.m_glStateManager.deleteSampler(glName);
}

gl::GLenum SamplerImpl::textureType() const
{
    switch (type)
    {
    case gl::GL_SAMPLER_2D: return gl::GL_TEXTURE_2D; break;
    case gl::GL_INT_SAMPLER_2D: return gl::GL_TEXTURE_2D; break;
    case gl::GL_UNSIGNED_INT_SAMPLER_2D: return gl::GL_TEXTURE_2D; break;
    case gl::GL_SAMPLER_CUBE: return gl::GL_TEXTURE_CUBE_MAP; break;

    default: Fail("Unsupported sampler type");
    }
}
}
