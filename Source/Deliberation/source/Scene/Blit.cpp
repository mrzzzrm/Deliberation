#include <Deliberation/Scene/Blit.h>

#include <glbinding/gl/enum.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>

namespace deliberation
{

Blit::Blit()
{

}

Blit::Blit(const Surface & source):
    m_source(source)
{
    m_viewport = Viewport(0, 0, source.width(), source.height());
}

Blit::Blit(const Surface & source, const Viewport & viewport):
    m_source(source),
    m_viewport(viewport)
{

}

void Blit::schedule()
{
    Assert(m_source.engaged(), "");

    if (!m_draw.engaged())
    {
        auto & context = m_source.get().context();

        m_draw.reset(context.createDraw(context.blitProgram(), gl::GL_TRIANGLE_STRIP));
        m_draw.get().addVertexBuffer(context.blitVertexBuffer());
        m_draw.get().sampler("Source").setTexture(m_source.get().texture());
        m_draw.get().state().setViewport(m_viewport);
    }

    m_draw.get().schedule();
}

}

