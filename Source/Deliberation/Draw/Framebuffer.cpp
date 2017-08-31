#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include "Detail/FramebufferImpl.h"

namespace deliberation
{
FramebufferDesc::FramebufferDesc(
    u32                                       width,
    u32                                       height,
    const std::vector<RenderTargetDesc> &     colorTargetDescs,
    const boost::optional<RenderTargetDesc> & depthTargetDesc,
    const std::string &                       name)
    : width(width)
    , height(height)
    , colorTargetDescs(colorTargetDescs)
    , depthTargetDesc(depthTargetDesc)
    , name(name)
{
    AssertM(
        !colorTargetDescs.empty() || depthTargetDesc,
        "Framebuffer must at least have one color target or a depth target");

    for (const auto & colorTargetDesc : colorTargetDescs)
    {
        AssertM(
            colorTargetDesc.format.pixelType() == PixelType_Color,
            "Color Target must be color type, is " +
                colorTargetDesc.format.toString());
        AssertM(!colorTargetDesc.name.empty(), "Color Target must be named");

        if (colorTargetDesc.format == PixelFormat_None)
        {
            AssertM(
                colorTargetDesc.surface.width() == width &&
                    colorTargetDesc.surface.height() == height,
                "Framebuffer/RenderTarget resolution mistach");
        }
    }

    if (depthTargetDesc && depthTargetDesc->format == PixelFormat_None)
    {
        AssertM(
            depthTargetDesc->surface.format().pixelType() == PixelType_Depth,
            "Depth target must be depth type, is " +
                depthTargetDesc->surface.format().toString());
        AssertM(depthTargetDesc->name.empty(), "Depth target must not be named");
        AssertM(
            depthTargetDesc->surface.width() == width &&
                depthTargetDesc->surface.height() == height,
            "Framebuffer/RenderTarget resolution mistach");
    }
}

Framebuffer::Framebuffer(const std::shared_ptr<FramebufferImpl> & impl)
    : Base(impl)
{
}

const std::string & Framebuffer::name() const
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    return m_impl->name;
}

unsigned int Framebuffer::width() const
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    return m_impl->width;
}

unsigned int Framebuffer::height() const
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    return m_impl->height;
}

bool Framebuffer::isBackbuffer() const
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    return m_impl->isBackbuffer;
}

const std::vector<RenderTarget> & Framebuffer::colorTargets() const
{
    AssertM(m_impl.get(), "Framebuffer is hollow");
    return m_impl->colorTargets;
}

boost::optional<Surface> Framebuffer::colorTarget(const std::string & name)
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    bool found;
    auto index = m_impl->colorTargetIndex(name, &found);

    if (!found) return {};

    return {m_impl->colorTargets[index].surface};
}

Surface & Framebuffer::colorTargetRef(const std::string & name)
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    bool found;
    auto index = m_impl->colorTargetIndex(name, &found);
    AssertM(found, "No such color target '" + name + "'");

    return m_impl->colorTargets[index].surface;
}

boost::optional<Surface> Framebuffer::depthTarget()
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    if (!m_impl->depthTarget) return {};

    return {m_impl->depthTarget->surface};
}

Surface & Framebuffer::depthTargetRef()
{
    AssertM((bool)m_impl->depthTarget, "No depth target");
    return m_impl->depthTarget->surface;
}

Clear & Framebuffer::clear()
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    if (!m_impl->clear)
    {
        m_impl->clear = m_impl->drawContext.createClear(*this);
    }

    return *m_impl->clear;
}

Clear Framebuffer::createClear()
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    return m_impl->drawContext.createClear(*this);
}

std::string Framebuffer::toString() const
{
    AssertM(m_impl.get(), "Framebuffer is hollow");

    return m_impl->toString();
}
}
