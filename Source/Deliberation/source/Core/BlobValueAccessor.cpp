#include <Deliberation/Core/BlobValueAccessor.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/DataLayoutField.h>

namespace deliberation
{

CBlobValueAccessor::CBlobValueAccessor(const Blob & data, const DataLayout & layout, const DataLayoutField & field):
    BlobValueAccessorBase{data, layout, field}
{

}

CBlobValue CBlobValueAccessor::operator[](size_t index) const
{
    return CBlobValue(m_data.ptr(m_field.offset() + index * m_layout.stride()), m_field.type());
}

BlobValueAccessor::BlobValueAccessor(Blob & data, const DataLayout & layout, const DataLayoutField & field):
    BlobValueAccessorBase{data, layout, field}
{

}

BlobValue BlobValueAccessor::operator[](size_t index)
{
    return BlobValue(m_data.ptr(m_field.offset() + index * m_layout.stride()), m_field.type());
}

CBlobValue BlobValueAccessor::operator[](size_t index) const
{
    return CBlobValue(m_data.ptr(m_field.offset() + index * m_layout.stride()), m_field.type());
}

}