#include <Deliberation/Draw/VertexAttribute.h>

#include <Deliberation/Draw/Detail/DrawImpl.h>

namespace deliberation
{

namespace detail
{

void SetVertexAttribute(DrawImpl & drawImpl, const ProgramInterfaceVertexAttribute & attribute, const void * data)
{
    drawImpl.setAttribute(attribute, data);
}

}

}