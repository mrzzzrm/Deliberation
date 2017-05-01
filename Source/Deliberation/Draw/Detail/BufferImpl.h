#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

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

    DrawContext &   drawContext;
    gl::GLuint      glName;
    size_t          count;
    DataLayout      layout;
    LayoutedBlob    mappedBlob;
    bool            mapped = false;
};

}

}

