#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/DataLayoutField.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

template<typename T>
TypedBlobIterator<T>::TypedBlobIterator(T * ptr, size_t stride):
    m_ptr(ptr),
    m_stride(stride)
{}

template<typename T>
void TypedBlobIterator<T>::put(const T & value)
{
    *m_ptr = std::move(value);
        
    m_ptr = (T*)(((u8*)m_ptr) + m_stride);
}

template<typename T>
CTypedBlobValueAccessor<T>::CTypedBlobValueAccessor(const Blob & data,
                                                    const DataLayout & layout,
                                                    const DataLayoutField & field):
    m_data(data, layout, field)
{
}

template<typename T>
const T & CTypedBlobValueAccessor<T>::operator[](size_t index) const
{
    Assert(m_data, "Accessor is not initialized");
    return m_data->data.access<T>(m_data->field.offset() + index * m_data->layout.stride());
}

template<typename T>
TypedBlobValueAccessor<T>::TypedBlobValueAccessor(Blob & data,
                                                  const DataLayout & layout,
                                                  const DataLayoutField & field):
    m_data(data, layout, field)
{
    Assert(field.type() == Type::resolve<T>(), std::string("Trying to access: ") + Type::resolve<T>().name() +
        ", but is actually " + field.type().name() + ": " + field.name());
}

template<typename T>
TypedBlobIterator<T> TypedBlobValueAccessor<T>::iterator()
{
    Assert(m_data, "Accessor is not initialized");
    return TypedBlobIterator<T>((T*)&m_data->data.access<T>(m_data->field.offset()), m_data->layout.stride());
}

template<typename T>
void TypedBlobValueAccessor<T>::assign(const std::vector<T> & values)
{
    Assert(m_data, "Accessor is not initialized");

    Assert(values.size() == m_data->data.size() / m_data->layout.stride(), "");

    for (int v = 0; v < values.size(); v++)
    {
        (*this)[v] = values[v];
    }
}

template<typename T>
T & TypedBlobValueAccessor<T>::operator[](size_t index)
{
    Assert(m_data, "Accessor is not initialized");
    return m_data->data.access<T>(m_data->field.offset() + index * m_data->layout.stride());
}

template<typename T>
const T & TypedBlobValueAccessor<T>::operator[](size_t index) const
{
    Assert(m_data, "Accessor is not initialized");
    return m_data->data.access<T>(m_data->field.offset() + index * m_data->layout.stride());
}

}
