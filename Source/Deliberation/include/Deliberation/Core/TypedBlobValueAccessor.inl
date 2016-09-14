#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/DataLayoutField.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

template<typename T>
CTypedBlobValueAccessor<T>::CTypedBlobValueAccessor(const Blob & data,
                                                    const DataLayout & layout,
                                                    const DataLayoutField & field):
    BlobValueAccessorBase{data, layout, field}
{
}

template<typename T>
const T & CTypedBlobValueAccessor<T>::operator[](size_t index) const
{
    return m_data.access<T>(m_field.offset() + index * m_layout.stride());
}

template<typename T>
TypedBlobValueAccessor<T>::TypedBlobValueAccessor(Blob & data,
                                                  const DataLayout & layout,
                                                  const DataLayoutField & field):
    BlobValueAccessorBase{data, layout, field}
{
    Assert(field.type() == Type::resolve<T>(), std::string("Trying to access: ") + Type::resolve<T>().name() + ", but is actually " + field.type().name() + ": " + field.name());
}

template<typename T>
void TypedBlobValueAccessor<T>::assign(const std::vector<T> & values)
{
    Assert(values.size() == m_data.size() / m_layout.stride(), "");

    for (int v = 0; v < values.size(); v++)
    {
        (*this)[v] = values[v];
    }
}

template<typename T>
T & TypedBlobValueAccessor<T>::operator[](size_t index)
{
    return m_data.access<T>(m_field.offset() + index * m_layout.stride());
}

template<typename T>
const T & TypedBlobValueAccessor<T>::operator[](size_t index) const
{
    return m_data.access<T>(m_field.offset() + index * m_layout.stride());
}

}
