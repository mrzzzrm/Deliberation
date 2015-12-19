#include <Deliberation/Draw/DrawOutput.h>

namespace deliberation
{

DrawOutput::DrawOutput() = default;

unsigned int DrawOutput::width() const
{
    return m_framebuffer.width();
}

unsigned int DrawOutput::height() const
{
    return m_framebuffer.height();
}

bool DrawOutput::isBackbuffer() const
{
    return m_framebuffer.isBackbuffer();
}

const Surface * DrawOutput::renderTarget(unsigned int index) const
{
    return m_framebuffer.surface(index);
}

const std::vector<Surface*> & DrawOutput::renderTargets() const
{
    return m_framebuffer.surfaces();
}

void DrawOutput::setRenderTarget(unsigned int index, Surface * surface)
{
    m_framebuffer.setSurface(index, surface);
}

}

