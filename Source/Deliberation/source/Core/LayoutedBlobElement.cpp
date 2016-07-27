#include <Deliberation/Core/LayoutedBlobElement.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{

CLayoutedBlobElement::CLayoutedBlobElement(const LayoutedBlob & blob, size_t index):
    Base(blob, index)
{
}

LayoutedBlobElement::LayoutedBlobElement(LayoutedBlob & blob, size_t index):
    Base(blob, index)
{
    Assert(m_index < m_blob.m_count, "");
}

LayoutedBlobElement & LayoutedBlobElement::operator=(const LayoutedBlobElement & rhs)
{
    Assert(m_blob.layout().equals(rhs.m_blob.layout()), "Incompatible layouts");

    auto stride = rhs.m_blob.layout().stride();
    auto * src = ((char*)rhs.m_blob.rawData().ptr()) + stride * rhs.m_index;

    m_blob.m_data.write(stride * m_index, src, stride);

    return *this;
}

LayoutedBlobElement & LayoutedBlobElement::operator=(const CLayoutedBlobElement & rhs)
{
    Assert(m_blob.layout().equals(rhs.m_blob.layout()), "Incompatible layouts");

    auto stride = rhs.m_blob.layout().stride();
    auto * src = ((char*)rhs.m_blob.rawData().ptr()) + stride * rhs.m_index;

    m_blob.m_data.write(stride * m_index, src, stride);

    return *this;
}

}