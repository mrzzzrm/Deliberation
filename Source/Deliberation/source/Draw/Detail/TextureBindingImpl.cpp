#include "TextureBindingImpl.h"

namespace deliberation
{

namespace detail
{

TextureBindingImpl::TextureBindingImpl(gl::GLenum type, gl::GLuint location):
    type(type),
    location(location),
    texture(nullptr)
{

}

}

}

