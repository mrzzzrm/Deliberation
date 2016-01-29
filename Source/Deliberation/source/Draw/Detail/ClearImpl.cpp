#include "ClearImpl.h"

#include <Deliberation/Draw/Context.h>

namespace deliberation
{

namespace detail
{

ClearImpl::ClearImpl(Context & context):
    context(context),
    color({0.0f, 0.0f, 0.0f, 0.0f}),
    depth(1.0f),
    stencil(0),
    clearColor(true),
    clearDepth(true),
    clearStencil(true),
    framebuffer(context.backbuffer())
{

}

}

}

