#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Core/DataLayout.h>

namespace deliberation
{

class Context;

namespace detail
{

class BufferImpl final
{
public:
    BufferImpl(Context & context, const DataLayout & layout);
    ~BufferImpl();

    Context &       context;
    gl::GLuint      glName;
    unsigned int    count;
    DataLayout      layout;
};

}

}

