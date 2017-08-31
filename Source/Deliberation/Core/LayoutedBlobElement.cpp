#include <Deliberation/Core/LayoutedBlobElement.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{
CLayoutedBlobElement::CLayoutedBlobElement(const LayoutedBlobElement & rhs)
    : Base(rhs.blob(), rhs.layout(), rhs.index())
{
}

CLayoutedBlobElement::CLayoutedBlobElement(
    const Blob & blob, const DataLayout & layout, size_t index)
    : Base(blob, layout, index)
{
}

LayoutedBlobElement::LayoutedBlobElement(
    Blob & blob, const DataLayout & layout, size_t index)
    : Base(blob, layout, index)
{
}

BlobValue LayoutedBlobElement::value(const DataLayoutField & field)
{
    return BlobValue(
        m_blob.ptr(m_layout.stride() * m_index + field.offset()), field.type());
}

BlobValue LayoutedBlobElement::value(const std::string & name)
{
    auto field = m_layout.field(name);
    return BlobValue(
        m_blob.ptr(m_layout.stride() * m_index + field.offset()), field.type());
}

LayoutedBlobElement & LayoutedBlobElement::
                      operator=(const LayoutedBlobElement & rhs)
{
    AssertM(m_layout.equals(rhs.m_layout), "Incompatible layouts");

    auto   stride = rhs.m_layout.stride();
    auto * src = rhs.m_blob.ptr(stride * rhs.m_index);

    m_blob.write(stride * m_index, src, stride);

    return *this;
}

LayoutedBlobElement & LayoutedBlobElement::
                      operator=(const CLayoutedBlobElement & rhs)
{
    AssertM(m_layout.equals(rhs.m_layout), "Incompatible layouts");

    auto   stride = rhs.m_layout.stride();
    auto * src = rhs.m_blob.ptr(stride * rhs.m_index);

    m_blob.write(stride * m_index, (void *)src, stride);

    return *this;
}
}