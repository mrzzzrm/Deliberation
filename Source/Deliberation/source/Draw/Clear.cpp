#include <Deliberation/Draw/Clear.h>

#include <Deliberation/Draw/Context.h>

#include "Detail/ClearImpl.h"

namespace deliberation
{

void Clear::setColor(const glm::vec4 & color)
{
    m_impl->color = color;
    m_impl->clearColor = true;
}

void Clear::disableColor()
{
    m_impl->clearColor = false;
}

void Clear::setDepth(float depth)
{
    m_impl->depth = depth;
    m_impl->clearDepth = true;
}

void Clear::disableDepth()
{
    m_impl->clearDepth = false;
}

void Clear::setStencil(gl::GLint stencil)
{
    m_impl->stencil = stencil;
    m_impl->clearStencil = true;
}

void Clear::disableStencil()
{
    m_impl->clearStencil = false;
}

void Clear::schedule()
{
    m_impl->context.scheduleClear(*this);
}

}

