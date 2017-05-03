#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include "Detail/FramebufferImpl.h"

namespace deliberation
{

FramebufferDesc::FramebufferDesc(
    u32 width,
    u32 height,
    const std::vector<RenderTargetDesc> & colorTargetDescs,
    const boost::optional<RenderTargetDesc> & depthTargetDesc
    ): width(width), height(height), colorTargetDescs(colorTargetDescs), depthTargetDesc(depthTargetDesc)
{
    Assert(!colorTargetDescs.empty() || depthTargetDesc,
        "Framebuffer must at least have one color target or a depth target")

    for (const auto & colorTargetDesc : colorTargetDescs)
    {
        Assert(!colorTargetDesc.name.empty(), "Color Target must be named");

        if (colorTargetDesc.format == PixelFormat_None)
        {
            Assert(colorTargetDesc.surface.width() == width &&
                   colorTargetDesc.surface.height() == height,
                   "Framebuffer/RenderTarget resolution mistach");
        }
    }

    if (depthTargetDesc && depthTargetDesc->format == PixelFormat_None)
    {
        Assert(depthTargetDesc->name.empty(), "Depth target must not be named");
        Assert(depthTargetDesc->surface.width() == width &&
                   depthTargetDesc->surface.height() == height,
               "Framebuffer/RenderTarget resolution mistach");
    }
}

Framebuffer::Framebuffer(const std::shared_ptr<FramebufferImpl> & impl):
    m_impl(impl)
{

}

unsigned int Framebuffer::width() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->width;
}

unsigned int Framebuffer::height() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->height;
}

bool Framebuffer::isBackbuffer() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->isBackbuffer;
}

boost::optional<Surface> Framebuffer::colorTarget(const std::string & name)
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    bool found;
    auto index = m_impl->colorTargetIndex(name, &found);

    if (!found) return {};

    return {m_impl->colorTargets[index].surface};
}

Surface & Framebuffer::colorTargetRef(const std::string & name)
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    bool found;
    auto index = m_impl->colorTargetIndex(name, &found);
    Assert(found, "No such color target '" + name + "'");

    return m_impl->colorTargets[index].surface;
}

boost::optional<Surface> Framebuffer::depthTarget()
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    if (!m_impl->depthTarget) return {};

    return {m_impl->depthTarget->surface};
}

Surface & Framebuffer::depthTargetRef()
{
    Assert((bool)m_impl->depthTarget, "No depth target");
    return m_impl->depthTarget->surface;
}

Clear & Framebuffer::clear()
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    if (!m_impl->clear)
    {
        m_impl->clear = m_impl->drawContext.createClear(*this);
    }

    return *m_impl->clear;
}

Clear Framebuffer::createClear()
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->drawContext.createClear(*this);
}

}

