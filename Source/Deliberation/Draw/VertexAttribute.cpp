#include <Deliberation/Draw/VertexAttribute.h>

#include <Deliberation/Draw/Draw.h>

namespace deliberation
{

namespace detail
{

void SetVertexAttribute(Draw & draw, const ProgramInterfaceVertexAttribute & attribute, const void * data)
{
    draw.setAttribute(attribute, data);
}

}

}