#include "TextureBindingImpl.h"

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

namespace detail
{

TextureBindingImpl::TextureBindingImpl(gl::GLenum uniformType, gl::GLuint location):
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
}

}

}

