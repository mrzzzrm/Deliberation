#include <Deliberation/Draw/Detail/UniformImpl.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Blob.h>

#include <Deliberation/Draw/Detail/UniformImpl.h>
#include <Deliberation/Draw/GL/GLSLTypeOf.h>

namespace deliberation
{
template<typename T>
void Uniform::set(const T & value)
{
    Assert((bool)m_drawImpl, "Hollow Uniform can't be set");
    Assert(Type::resolve<T>() == m_field->type(),
           std::string("Uniform type mismatch. Got ") + Type::resolve<T>().name() +
               " where " + m_field->type().name() + " was required.");

    set(&value, sizeof(value));
}

template<typename T>
void Uniform::set(const std::vector<T> & vec)
{
    Assert((bool)m_drawImpl, "Hollow Uniform can't be set");
    Assert(Type::resolve<T>() == m_field->type(), "Uniform type mismatch");
    Assert(
        vec.size() <= m_field->arraySize(),
        "Bounds exceeded or Uniform not an array");

    set(vec.data(), sizeof(T) * vec.size());
}
}
