#include <Deliberation/Draw/Uniform.h>

namespace deliberation
{

Uniform::Uniform():
    m_impl(nullptr)
{
}

Uniform::Uniform(UniformImpl & impl):
    m_impl(&impl)
{

}

void Uniform::set(const LayoutedBlob & array)
{
    Assert(m_impl, "Hollow Uniform can't be set");
    Assert(array.layout().fields().size() == 1, "Uniform are primitive type structs");
    Assert(array.layout().fields()[0].type() == m_impl->type, "Uniform type mismatch");
    Assert(array.count() <= m_impl->arraySize, "Bounds exceeded or Uniform not an array");

    m_impl->blob = array.rawData();
    m_impl->isAssigned = true;
    m_impl->count = array.count();
}

}


