#include "SamplerImpl.h"

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

namespace detail
{

SamplerImpl::SamplerImpl(gl::GLuint glName, gl::GLenum uniformType, gl::GLuint location):
    glName(glName),
    type(uniformType),
    location(location),
    texture(nullptr)
{
    wrap[0] = gl::GL_REPEAT;
    wrap[1] = gl::GL_REPEAT;
    wrap[2] = gl::GL_REPEAT;
}

gl::GLenum SamplerImpl::textureType() const
{
    switch(type)
    {
    case gl::GL_SAMPLER_2D:     return gl::GL_TEXTURE_2D; break;
    case gl::GL_SAMPLER_CUBE:   return gl::GL_TEXTURE_CUBE_MAP; break;

    default:
        Fail("Unsupported sampler type");
    }
}

}

}

