#include "BufferImpl.h"

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

namespace detail
{

BufferImpl::BufferImpl(DrawContext & drawContext, const DataLayout & layout):
    drawContext(drawContext),
    glName(0u),
    count(0u),
    layout(layout)
{
    drawContext.allocateBuffer(*this);
}

BufferImpl::~BufferImpl()
{
    drawContext.deallocateBuffer(*this);
}

}

}

