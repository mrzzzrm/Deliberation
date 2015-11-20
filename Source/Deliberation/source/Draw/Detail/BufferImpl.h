#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/BufferLayout.h>

namespace deliberation
{

class Context;

namespace detail
{

class BufferImpl final
{
public:
    BufferImpl(Context & context);
    ~BufferImpl();

    Context & context;
    gl::GLuint glName;
    unsigned int count;
    BufferLayout layout;
};

}

}

