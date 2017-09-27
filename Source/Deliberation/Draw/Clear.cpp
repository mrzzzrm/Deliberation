#include <Deliberation/Draw/Clear.h>

#include <glbinding/gl/gl.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include "Detail/ClearImpl.h"
#include "Detail/FramebufferImpl.h"

namespace deliberation
{
Clear::Clear(const std::shared_ptr<ClearImpl> & impl) : Base(impl) {}

const Framebuffer & Clear::framebuffer() const
{
    AssertM(m_impl.get(), "Clear is hollow");

    return m_impl->framebuffer;
}

void Clear::setColor(const glm::vec4 & color)
{
    AssertM(m_impl.get(), "Clear is hollow");

    for (auto & color2 : m_impl->colors)
    {
        color2 = color;
    }
}

void Clear::setColor(const std::string & name, const glm::vec4 & color)
{
    AssertM(m_impl.get(), "Clear is hollow");

    const auto index = m_impl->framebuffer.m_impl->colorTargetIndex(name);

    m_impl->colors[index] = color;
}

void Clear::disableColor(const std::string & name)
{
    AssertM(m_impl.get(), "Clear is hollow");

    const auto index = m_impl->framebuffer.m_impl->colorTargetIndex(name);
    m_impl->colors[index] = boost::optional<glm::vec4>();
}

void Clear::disableColor()
{
    AssertM(m_impl.get(), "Clear is hollow");

    for (auto & color2 : m_impl->colors)
    {
        color2 = boost::optional<glm::vec4>();
    }
}

void Clear::setDepth(float depth)
{
    AssertM(m_impl.get(), "Clear is hollow");
    AssertM(
        (bool)m_impl->framebuffer.depthTarget(),
        "Can't set depth, because there is no depth target in Framebuffer");

    m_impl->depth = depth;
}

void Clear::disableDepth()
{
    AssertM(m_impl.get(), "Clear is hollow");

    m_impl->depth = boost::optional<float>();
}

void Clear::setStencil(u32 stencil)
{
    AssertM(m_impl.get(), "Clear is hollow");
    AssertM(
        m_impl->framebuffer.depthTarget() &&
            m_impl->framebuffer.depthTarget()->format().numStencilBits() > 0,
        "Can't set stencil, because there are no stencil bits in the "
        "Framebuffer");

    m_impl->stencil = stencil;
}

void Clear::disableStencil()
{
    AssertM(m_impl.get(), "Clear is hollow");

    m_impl->stencil = boost::optional<gl::GLint>();
}

void Clear::render()
{
    auto & framebufferImpl = *m_impl->framebuffer.m_impl;

    auto glStateManager = GetGlobal<DrawContext>()->m_glStateManager;

    framebufferImpl.bind(m_impl->drawBuffers);

    glStateManager.setViewport(
        0, 0, framebufferImpl.width, framebufferImpl.height);
    glStateManager.setScissor(
        0, 0, framebufferImpl.width, framebufferImpl.height);

    glStateManager.setDepthMask(true);
    glStateManager.setStencilMask(~0);
    glStateManager.setClearStencil(0);

    if (framebufferImpl.isBackbuffer)
    {
        Assert(m_impl->colors.size() == 1);

        auto flags = gl::ClearBufferMask::GL_NONE_BIT;

        if (m_impl->colors[0])
        {
            auto & c = *m_impl->colors[0];
            glStateManager.setClearColor(c.r, c.g, c.b, c.a);
            flags |= gl::GL_COLOR_BUFFER_BIT;
        }
        if (m_impl->depth)
        {
            glStateManager.setClearDepth(*m_impl->depth);
            flags |= gl::GL_DEPTH_BUFFER_BIT;
        }
        if (m_impl->stencil)
        {
            glStateManager.setClearStencil(*m_impl->stencil);
            flags |= gl::GL_STENCIL_BUFFER_BIT;
        }

        if (flags != gl::ClearBufferMask::GL_NONE_BIT)
        {
            gl::glClear(flags);
        }
    }
    else
    {
        AssertM(
            m_impl->colors.size() == framebufferImpl.colorTargets.size(), "");

        glStateManager.setDrawBuffers(m_impl->drawBuffers);

        auto & impl = *m_impl;

        for (auto b = 0u; b < framebufferImpl.colorTargets.size(); b++)
        {
            if (m_impl->colors[b])
            {
                auto color = *m_impl->colors[b];
                gl::glClearBufferfv(gl::GL_COLOR, b, &color[0]);
            }
        }

        if (impl.depth)
        {
            AssertM(
                (bool)framebufferImpl.depthTarget,
                "Can't clear depth: No depth target in Framebuffer");
            gl::glClearBufferfv(gl::GL_DEPTH, 0, &*impl.depth);
        }

        if (impl.stencil)
        {
            AssertM(
                (bool)framebufferImpl.depthTarget,
                "Can't clear stencil: No depth target in Framebuffer");
            gl::glClearBufferiv(gl::GL_STENCIL, 0, (gl::GLint *)&*impl.stencil);
        }
    }
}
}
