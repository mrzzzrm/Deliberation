#include <Deliberation/Draw/Detail/UniformImpl.h>

namespace deliberation
{

namespace detail
{

UniformImpl::UniformImpl():
    type(gl::GL_NONE),
    location((unsigned int)-1),
    isPlaceholder(true),
    isAssigned(false)
{

}

UniformImpl::UniformImpl(gl::GLenum type, unsigned int location):
    type(type),
    location(location),
    isPlaceholder(false),
    isAssigned(false)
{

}

}

}

