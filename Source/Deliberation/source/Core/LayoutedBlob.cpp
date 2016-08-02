#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

LayoutedBlob::LayoutedBlob()
{

}

LayoutedBlob::LayoutedBlob(const DataLayout & layout, size_t count):
    m_layout(layout),
    m_count(count),
    m_data(count * m_layout.stride())
{

}

const DataLayout & LayoutedBlob::layout() const
{
    return m_layout;
}

size_t LayoutedBlob::count() const
{
    return m_count;
}

const Blob & LayoutedBlob::rawData() const
{
    return m_data;
}

BlobValueAccessor LayoutedBlob::field(const std::string & name)
{
    return field(m_layout.field(name));
}

CBlobValueAccessor LayoutedBlob::field(const std::string & name) const
{
    return field(m_layout.field(name));
}

BlobValueAccessor LayoutedBlob::field(const DataLayoutField & field)
{
    return BlobValueAccessor(m_data, m_layout, field);
}

CBlobValueAccessor LayoutedBlob::field(const DataLayoutField & field) const
{
    return CBlobValueAccessor(m_data, m_layout, field);
}

void LayoutedBlob::append(const CLayoutedBlobElement & element)
{
    m_count++;
    m_data.resize(m_layout.stride() * m_count);
    operator[](m_count - 1) = element;
}

LayoutedBlobElement LayoutedBlob::operator[](size_t index)
{
    Assert(index < m_count, "");

    return LayoutedBlobElement(m_data, m_layout, index);
}

CLayoutedBlobElement LayoutedBlob::operator[](size_t index) const
{
    Assert(index < m_count, "");

    return CLayoutedBlobElement(m_data, m_layout, index);
}

std::string LayoutedBlob::toString() const
{
    return "";
}

}