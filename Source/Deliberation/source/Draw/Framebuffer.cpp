#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>

#include "Detail/FramebufferImpl.h"

namespace deliberation
{

Framebuffer::Framebuffer() = default;

Framebuffer::~Framebuffer() = default;

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

Surface * Framebuffer::depthTarget() const
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

Clear Framebuffer::createClear()
{
    Assert(m_impl.get(), "Framebuffer is hollow");

    return m_impl->context().createClear(*this);
}

Framebuffer::Framebuffer(const std::shared_ptr<detail::FramebufferImpl> & impl):
    m_impl(impl)
{

}

}

