#pragma once

#include <glbinding/gl/types.h>

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{
class DrawContext;

class BufferImpl final
{
public:
    BufferImpl(const DataLayout & layout);
    ~BufferImpl();

    gl::GLuint    glName;
    size_t        count;
    DataLayout    layout;
};
}
