#include <Deliberation/Draw/Clear.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>

#include "Detail/ClearImpl.h"

namespace deliberation
{

Clear::Clear() = default;

Framebuffer & Clear::framebuffer()
{
    Assert(m_impl.get(), "Clear is hollow");
    return m_impl->framebuffer;
}

const Framebuffer & Clear::framebuffer() const
{
    Assert(m_impl.get(), "Clear is hollow");
    return m_impl->framebuffer;
}

void Clear::setFramebuffer(Framebuffer & framebuffer)
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->framebuffer = framebuffer;
}

void Clear::setColor(const glm::vec4 & color)
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->color = color;
    m_impl->clearColor = true;
}

void Clear::setColor(unsigned int rt, const glm::vec4 & color)
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->colorOverrides[rt] = color;
    m_impl->clearColor = true;
}

void Clear::disableColor()
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->clearColor = false;
}

void Clear::setDepth(float depth)
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->depth = depth;
    m_impl->clearDepth = true;
}

void Clear::disableDepth()
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->clearDepth = false;
}

void Clear::setStencil(gl::GLint stencil)
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->stencil = stencil;
    m_impl->clearStencil = true;
}

void Clear::disableStencil()
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->clearStencil = false;
}

void Clear::schedule()
{
    Assert(m_impl.get(), "Clear is hollow");
    m_impl->context.scheduleClear(*this);
}

Clear::Clear(const std::shared_ptr<detail::ClearImpl> & impl):
    m_impl(impl)
{

}

}

