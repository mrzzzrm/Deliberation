#include <Deliberation/Core/LayoutedBlob.h>

namespace deliberation
{

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

std::string LayoutedBlob::toString() const
{
    return "";
}

}