#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Core/DataLayout.h>

namespace deliberation
{

class DrawContext;

namespace detail
{

class BufferImpl final
{
public:
    BufferImpl(DrawContext & drawContext, const DataLayout & layout);
    ~BufferImpl();

    DrawContext &       drawContext;
    gl::GLuint      glName;
    unsigned int    count;
    DataLayout      layout;
};

}

}

