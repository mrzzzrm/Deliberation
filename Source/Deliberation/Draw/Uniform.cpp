#include <Deliberation/Draw/Detail/DrawImpl.h>
#include <Deliberation/Draw/Uniform.h>

namespace deliberation
{
Uniform::Uniform(const std::shared_ptr<DrawImpl> & drawImpl, size_t index)
    : m_drawImpl(drawImpl)
    , m_index(index)
    , m_field(&m_drawImpl->uniformLayout.field(index))
{
}

void Uniform::set(const LayoutedBlob & array)
{
    Assert((bool)m_drawImpl, "Hollow uniform");
    Assert(
        array.layout().fields().size() == 1,
        "Uniform are primitive type structs");

    Assert(
        array.layout().fields()[0].type() == m_field->type(),
        "Uniform type mismatch");
    Assert(
        array.count() <= m_field->arraySize(),
        "Bounds exceeded or Uniform not an array");

    set(array.rawData().ptr(), array.rawData().size());
}

void Uniform::set(const void * data, size_t size)
{
    Assert(
        size <= m_field->type().size() * m_field->arraySize(), "Out of range");
    Assert(size % m_field->type().size() == 0, "Incomplete array field");

    m_drawImpl->uniformData.write(m_field->offset(), data, size);
    m_drawImpl->uniforms[m_index].assigned = true;
    m_drawImpl->uniforms[m_index].count = size / m_field->type().size();
}
}
