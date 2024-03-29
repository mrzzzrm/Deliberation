#pragma once

#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DrawImpl;

class VertexAttribute final
{
public:
    VertexAttribute(
        const std::shared_ptr<DrawImpl> &       drawImpl,
        const ProgramInterfaceVertexAttribute & attribute);

    template<typename T>
    void set(const T & value);

    void set(const void * data);

private:
    std::shared_ptr<DrawImpl>               m_drawImpl;
    const ProgramInterfaceVertexAttribute & m_attribute;
};
}

#include <Deliberation/Draw/VertexAttribute.inl>