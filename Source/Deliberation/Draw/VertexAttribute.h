#pragma once

#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class DrawImpl;

namespace detail
{
void SetVertexAttribute(DrawImpl & drawImpl,
                        const ProgramInterfaceVertexAttribute & attribute,
                        const void * data);
}

template<typename T>
class VertexAttribute final
{
public:
    VertexAttribute(const std::shared_ptr<DrawImpl> & drawImpl,
                    const ProgramInterfaceVertexAttribute & attribute);

    void set(const T & value);

private:
    std::shared_ptr<DrawImpl>               m_drawImpl;
    const ProgramInterfaceVertexAttribute & m_attribute;
};

}

#include <Deliberation/Draw/VertexAttribute.inl>