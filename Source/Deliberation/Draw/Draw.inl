#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/ProgramInterface.h>
#include <Deliberation/Draw/VertexAttribute.h>

namespace deliberation
{
template<typename T>
void Draw::setAttribute(const std::string & name, const T & value)
{
    Assert(m_impl.get(), "Can't perform action on hollow Draw");

    const auto & attribute = program().interface().attributeRef(name);
    Assert(
        attribute.type() == Type::resolve<T>(),
        std::string("Vertex attribute type mismatch: '") +
            attribute.type().name() + "' and '" + Type::resolve<T>().name() +
            "'");

    setAttribute(attribute, (const void *)&value);
}
}
