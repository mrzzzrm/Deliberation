#include <sstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>

namespace deliberation
{
template<typename T>
T & LayoutedBlobElementBase<T>::blob() const
{
    return m_blob;
}

template<typename T>
const DataLayout & LayoutedBlobElementBase<T>::layout() const
{
    return m_layout;
}

template<typename T>
size_t LayoutedBlobElementBase<T>::index() const
{
    return m_index;
}

template<typename T>
CBlobValue
LayoutedBlobElementBase<T>::value(const DataLayoutField & field) const
{
    return CBlobValue(
        m_blob.ptr(m_layout.stride() * m_index + field.offset()), field.type());
}

template<typename T>
CBlobValue LayoutedBlobElementBase<T>::value(const std::string & name) const
{
    auto field = m_layout.field(name);
    return CBlobValue(
        m_blob.ptr(m_layout.stride() * m_index + field.offset()), field.type());
}

template<typename T>
std::string LayoutedBlobElementBase<T>::toString() const
{
    std::stringstream stream;
    stream << "{";
    for (auto & field : m_layout.fields())
    {
        stream << value(field).toString() << ", ";
    }
    stream << "}";
    return stream.str();
}

template<typename T>
LayoutedBlobElementBase<T>::LayoutedBlobElementBase(
    T & blob, const DataLayout & layout, size_t index)
    : m_blob(blob), m_layout(layout), m_index(index)
{
}
}
