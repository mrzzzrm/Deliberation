#include <Deliberation/Draw/Detail/UniformImpl.h>

namespace deliberation
{

UniformImpl::UniformImpl():
    type(Type_None),
    location((unsigned int)-1),
    isPlaceholder(true),
    isAssigned(false),
    arraySize(0),
    count(0)
{

}

UniformImpl::UniformImpl(Type type, unsigned int location, unsigned int arraySize):
    type(type),
    location(location),
    isPlaceholder(false),
    isAssigned(false),
    arraySize(arraySize),
    count(0)
{

}

}

