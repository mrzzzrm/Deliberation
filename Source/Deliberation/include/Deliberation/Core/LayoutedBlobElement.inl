#include <Deliberation/Core/Assert.h>

namespace deliberation
{

template<typename T>
T & LayoutedBlobElementBase<T>::blob() const
{
    return m_blob;
}

template<typename T>
size_t LayoutedBlobElementBase<T>::index() const
{
    return m_index;
}

template<typename T>
LayoutedBlobElementBase<T>::LayoutedBlobElementBase(T & blob, size_t index):
    m_blob(blob),
    m_index(index)
{
    Assert(m_index < m_blob.m_count, "");
}

}
