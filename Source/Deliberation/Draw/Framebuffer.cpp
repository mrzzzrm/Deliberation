#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include "Detail/FramebufferImpl.h"

namespace deliberation
{

unsigned int Framebuffer::width() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->width();
}

unsigned int Framebuffer::height() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->height();
}

bool Framebuffer::isBackbuffer() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->isBackbuffer();
}

Surface * Framebuffer::renderTarget(unsigned int index)
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->renderTarget(index);
}

const Surface * Framebuffer::renderTarget(unsigned int index) const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->renderTarget(index);
}

const std::vector<Optional<Surface>> & Framebuffer::renderTargets() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->renderTargets();
}

Surface * Framebuffer::depthTarget()
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->depthTarget();
}

const Surface * Framebuffer::depthTarget() const
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->depthTarget();
}

void Framebuffer::setRenderTarget(unsigned int index, Surface * surface)
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    m_impl->setRenderTarget(index, surface);
}

void Framebuffer::setDepthTarget(Surface * surface)
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    m_impl->setDepthTarget(surface);
}

void Framebuffer::addRenderTarget(PixelFormat format, int index)
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    m_impl->addRenderTarget(format, index);
}

void Framebuffer::addDepthTarget(PixelFormat format)
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    m_impl->addDepthTarget(format);
}

Clear & Framebuffer::clear()
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    if (!m_impl->m_clear.engaged())
    {
        m_impl->m_clear.reset(m_impl->m_drawContext.createClear(*this));
    }

    return m_impl->m_clear.get();
}

Clear Framebuffer::createClear()
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->drawContext().createClear(*this);
}

Framebuffer::Framebuffer(const std::shared_ptr<detail::FramebufferImpl> & impl):
    m_impl(impl)
{

}

}

