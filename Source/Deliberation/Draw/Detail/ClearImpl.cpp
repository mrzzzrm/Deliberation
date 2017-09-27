#include "ClearImpl.h"

#include <Deliberation/Draw/Detail/FramebufferImpl.h>
#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{
ClearImpl::ClearImpl(
    const std::shared_ptr<FramebufferImpl> & framebufferImpl)
    : framebuffer(framebufferImpl)
{
    if (framebufferImpl->isBackbuffer)
    {
        colors.resize(1);
        colors[0] = glm::vec4(0.0f);
        depth = 1.0f;
    }
    else
    {
        colors.resize(framebufferImpl->colorTargets.size());

        if (framebufferImpl->depthTarget) depth = 1.0f;

        drawBuffers.reserve(colors.size());
        for (size_t b = 0; b < colors.size(); b++)
        {
            colors[b] = glm::vec4(0.0f);
            drawBuffers.emplace_back(
                (gl::GLenum)((u32)gl::GL_COLOR_ATTACHMENT0 + b));
        }
    }
}
}
