#include <Deliberation/Draw/VertexAttribute.h>
#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>

namespace deliberation
{

template<typename T>
void Draw::setAttribute(const std::string & name, const T & value)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    const auto & attribute = program().interface().attributeRef(name);
    Assert(attribute.type() == Type::resolve<T>(), std::string("Vertex attribute type mismatch: '") +
        attribute.type().name() + "' and '" + Type::resolve<T>().name() + "'");

    setAttribute(attribute, (const void *)&value);
}

template<typename T>
void Draw::setAttribute(size_t index, const T & value)
{
    const auto & attributes = program().interface().attributes();

    Assert(index < attributes.size(), "Attribute index out of range");

    setAttribute(attributes[index], (const void *)&value);
}

template<typename T>
VertexAttribute<T> Draw::attribute(const std::string & name)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    const auto & attribute = program().interface().attributeRef(name);
    Assert(attribute.type() == Type::resolve<T>(), std::string("Vertex attribute type mismatch: '") +
        attribute.type().name() + "' and '" + Type::resolve<T>().name() + "'");

    return VertexAttribute<T>(*this, attribute);
}

}
