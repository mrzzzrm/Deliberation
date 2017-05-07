#include <Deliberation/Draw/VertexAttribute.h>

#include <Deliberation/Draw/Detail/DrawImpl.h>

namespace deliberation
{

VertexAttribute::VertexAttribute(const std::shared_ptr<DrawImpl> & drawImpl,
                                 const ProgramInterfaceVertexAttribute & attribute):
    m_drawImpl(drawImpl),
    m_attribute(attribute)
{

}

void VertexAttribute::set(const void * data)
{
    m_drawImpl->setAttribute(m_attribute, data);
}

}