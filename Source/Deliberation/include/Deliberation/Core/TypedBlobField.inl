#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/DataLayoutField.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/IntTypes.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

template<typename T>
TypedBlobField<T>::TypedBlobField(Blob & data, const DataLayout & layout, const DataLayoutField & field):
    m_data(data),
    m_layout(layout),
    m_field(field)
{
    Assert(field.type().typeID() == TypeID::value<T>(), std::string("Field: ") + field.type().name() + " " + field.name());
}

template<typename T>
void TypedBlobField<T>::assign(const std::vector<T> & values)
{
    Assert(values.size() == m_data.size() / m_layout.stride(), "");

    for (int v = 0; v < values.size(); v++)
    {
        (*this)[v] = values[v];
    }
}

template<typename T>
T & TypedBlobField<T>::operator[](size_t index)
{
    return m_data.access<T>(m_field.offset() + index * m_layout.stride());
}

template<typename T>
const T & TypedBlobField<T>::operator[](size_t index) const
{
    return m_data.access<T>(m_field.offset() + index * m_layout.stride());
}

}
