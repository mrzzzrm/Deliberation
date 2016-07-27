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

LayoutedBlobElement LayoutedBlob::operator[](size_t index)
{
    Assert(index < m_count, "");

    return LayoutedBlobElement(*this, index);
}

CLayoutedBlobElement LayoutedBlob::operator[](size_t index) const
{
    Assert(index < m_count, "");

    return CLayoutedBlobElement(*this, index);
}

std::string LayoutedBlob::toString() const
{
    return "";
}

}