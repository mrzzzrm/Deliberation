#include <Deliberation/Draw/DrawOutput.h>

#include "Detail/ProgramImpl.h"

namespace deliberation
{

DrawOutput::DrawOutput(const DrawOutput & other):
    m_program(other.m_program)
{

}

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

void DrawOutput::setRenderTarget(const std::string & name, Surface * surface)
{
    auto index = m_program.interface.fragmentOutput(name).location();
    m_framebuffer.setSurface(index, surface);
}

void DrawOutput::setDepthTarget(Surface * surface)
{
    m_framebuffer.setDepthSurface(surface);
}

DrawOutput::DrawOutput(const detail::ProgramImpl & program):
    m_program(program)
{

}

}

