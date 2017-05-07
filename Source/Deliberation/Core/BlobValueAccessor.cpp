#include <Deliberation/Core/BlobValueAccessor.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/DataLayoutField.h>

namespace deliberation
{
CBlobValueAccessor::CBlobValueAccessor(
    const Blob & data, const DataLayout & layout, const DataLayoutField & field)
    : m_data(data, layout, field)
{
}

CBlobValue CBlobValueAccessor::operator[](size_t index) const
{
    Assert(m_data, "Accessor is not initialized");
    return CBlobValue(
        m_data->data.ptr(
            m_data->field.offset() + index * m_data->layout.stride()),
        m_data->field.type());
}

BlobValueAccessor::BlobValueAccessor(
    Blob & data, const DataLayout & layout, const DataLayoutField & field)
    : m_data{data, layout, field}
{
}

BlobValue BlobValueAccessor::operator[](size_t index)
{
    Assert(m_data, "Accessor is not initialized");
    return BlobValue(
        m_data->data.ptr(
            m_data->field.offset() + index * m_data->layout.stride()),
        m_data->field.type());
}

CBlobValue BlobValueAccessor::operator[](size_t index) const
{
    Assert(m_data, "Accessor is not initialized");
    return CBlobValue(
        m_data->data.ptr(
            m_data->field.offset() + index * m_data->layout.stride()),
        m_data->field.type());
}
}