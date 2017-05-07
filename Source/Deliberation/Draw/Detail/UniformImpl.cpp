#include <Deliberation/Draw/Detail/UniformImpl.h>

namespace deliberation
{

UniformImpl::UniformImpl(Type type, unsigned int location, unsigned int arraySize):
    type(type),
    location(location),
    isAssigned(false),
    arraySize(arraySize),
    count(0)
{

}

}

