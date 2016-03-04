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
    Assert(m_impl, "Hollow Uniform can't be set");
    Assert(GLSLTypeOf<T>::value == m_impl->type, "Uniform type mismatch");

    m_impl->blob = Blob::fromValue(value);
    m_impl->isAssigned = true;
    m_impl->count = 1;
}

template<typename T>
void Uniform::set(const std::vector<T> & vec)
{
    Assert(m_impl, "Hollow Uniform can't be set");
    Assert(GLSLTypeOf<T>::value == m_impl->type, "Uniform type mismatch");
    Assert(vec.size() <= m_impl->arraySize, "Bounds exceeded or Uniform not an array");

    m_impl->blob = Blob(vec);
    m_impl->isAssigned = true;
    m_impl->count = vec.size();
}

}


