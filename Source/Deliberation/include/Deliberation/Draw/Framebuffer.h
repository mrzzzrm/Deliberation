#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/Surface.h>

namespace deliberation
{

namespace detail
{
    class DrawExecution;
    class DrawImpl;
    class ProgramImpl;
    class FramebufferImpl;
}

class ClearExecution;
class Context;
class Surface;

class DELIBERATION_API Framebuffer final
{
public:
    Framebuffer();
    ~Framebuffer();

    unsigned int width() const;
    unsigned int height() const;

    bool isBackbuffer() const;

    const Surface * renderTarget(unsigned int index) const;
    const std::vector<Optional<Surface>> & renderTargets() const;

    Surface * depthTarget() const;

    void setRenderTarget(unsigned int index, Surface * surface);

    void setDepthTarget(Surface * surface);

private:
    friend class Context;
    friend class ClearExecution;
    friend class detail::DrawExecution;
    friend class detail::DrawImpl;

private:
    Framebuffer(const std::shared_ptr<detail::FramebufferImpl> & impl);

private:
    std::shared_ptr<detail::FramebufferImpl> m_impl;
};

}

