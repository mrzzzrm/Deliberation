#include <Deliberation/Draw/Detail/UniformImpl.h>

namespace deliberation
{

namespace detail
{

UniformImpl::UniformImpl():
    type(gl::GL_NONE),
    location((unsigned int)-1),
    isPlaceholder(true),
    isAssigned(false),
    arraySize(0),
    count(0)
{

}

UniformImpl::UniformImpl(gl::GLenum type, unsigned int location, unsigned int arraySize):
    type(type),
    location(location),
    isPlaceholder(false),
    isAssigned(false),
    arraySize(arraySize),
    count(0)
{

}

}

}

