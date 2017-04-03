#pragma once

#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Draw;

namespace detail
{
void SetVertexAttribute(Draw & draw, const ProgramInterfaceVertexAttribute & attribute, const void * data);
}

template<typename T>
class VertexAttribute final
{
public:
    VertexAttribute(Draw & draw, const ProgramInterfaceVertexAttribute & attribute);

    void set(const T & value);

private:
    void set(const void * ptr);

private:
    Draw &                                  m_draw;
    const ProgramInterfaceVertexAttribute & m_attribute;
};

}

#include <Deliberation/Draw/VertexAttribute.inl>