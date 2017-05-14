#include "SamplerImpl.h"

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
SamplerImpl::SamplerImpl(
    gl::GLenum uniformType, gl::GLenum valueType, gl::GLuint location)
    : glSampler(), type(uniformType), valueType(valueType), location(location)
{
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