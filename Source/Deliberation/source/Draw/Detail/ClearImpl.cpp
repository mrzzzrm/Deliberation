#include "ClearImpl.h"

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

namespace detail
{

ClearImpl::ClearImpl(DrawContext & context,
                     const std::shared_ptr<FramebufferImpl> & framebuffer):
    context(context),
    color({0.0f, 0.0f, 0.0f, 0.0f}),
    depth(1.0f),
    stencil(0),
    clearColor(true),
    clearDepth(true),
    clearStencil(true),
    framebuffer(framebuffer)
{

}

}

}

