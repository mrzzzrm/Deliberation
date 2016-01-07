#pragma once

#include <glbinding/gl/enum.h>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Blob.h>

namespace deliberation
{

namespace detail
{

class DELIBERATION_API UniformImpl final
{
public:
    UniformImpl();
    UniformImpl(gl::GLenum type, unsigned int location);

    gl::GLenum      type;
    unsigned int    location;
    bool            isAssigned;
    bool            isPlaceholder; // for sparse uniform layout.
    Blob            blob;
};

}

}

