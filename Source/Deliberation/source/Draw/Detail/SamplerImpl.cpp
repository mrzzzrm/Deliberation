#include "SamplerImpl.h"

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

namespace detail
{

SamplerImpl::SamplerImpl(gl::GLuint glName, gl::GLenum uniformType, gl::GLuint location):
    glName(glName),
    type(gl::GL_NONE),
    location(location),
    texture(nullptr)
{
    switch(uniformType)
    {
    case gl::GL_SAMPLER_2D:     type = gl::GL_TEXTURE_2D; break;
    case gl::GL_SAMPLER_CUBE:   type = gl::GL_TEXTURE_CUBE_MAP; break;

    default:
        Fail("Unsupported sampler type");
    }

    wrap[0] = gl::GL_REPEAT;
    wrap[1] = gl::GL_REPEAT;
    wrap[2] = gl::GL_REPEAT;
}

}

}

