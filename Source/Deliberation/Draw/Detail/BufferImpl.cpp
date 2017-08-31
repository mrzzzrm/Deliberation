#include "BufferImpl.h"

#include <glbinding/gl/gl.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{
BufferImpl::BufferImpl(DrawContext & drawContext, const DataLayout & layout)
    : drawContext(drawContext), count(0u), layout(layout)
{
    gl::glGenBuffers(1, &glName);
    AssertM(glName != 0u, "Failed to create GL BufferObject");
}

BufferImpl::~BufferImpl() { drawContext.m_glStateManager.deleteBuffer(glName); }
}
