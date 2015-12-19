#pragma once

#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/Detail/Framebuffer.h>

#include <Deliberation/Draw/GL/GLFramebufferDesc.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
}

class Surface;

class DELIBERATION_API DrawOutput
{
public:
    DrawOutput();

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    const Surface * renderTarget(unsigned int index) const;
    const std::vector<Surface*> & renderTargets() const;

    void setRenderTarget(unsigned int index, Surface * surface);

private:
    friend class detail::DrawExecution;

private:
    detail::Framebuffer m_framebuffer;
};

}

