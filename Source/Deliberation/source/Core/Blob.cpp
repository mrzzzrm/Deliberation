#include <Deliberation/Core/Blob.h>

namespace deliberation
{

Blob::Blob():
    m_impl(nullptr)
{
}

Blob::Blob(std::size_t size):
    m_impl(std::unique_ptr<detail::IBlobImpl>(new detail::VectorBlobImpl<char>(size)))
{

}

Blob::Blob(const Blob & blob):
    m_impl(blob.m_impl ? blob.m_impl->clone() : nullptr)
{
}

Blob::Blob(Blob && blob):
    m_impl(blob.m_impl ? blob.m_impl->clone() : nullptr)
{

}

std::size_t Blob::size() const
{
    return m_impl ? m_impl->size() : 0u;
}

void * Blob::ptr(size_t offset)
{
    Assert(offset < size(), "");

    return m_impl ? ((char*)m_impl->ptr()) + offset : nullptr;
}

const void * Blob::ptr(size_t offset) const
{
    Assert(offset < size(), "");

    return m_impl ? ((char*)m_impl->ptr()) + offset : nullptr;
}

void Blob::write(std::size_t offset, void * src, size_t length)
{
    Assert(offset + length <= size(), "");
    Assert(ptr(), "");

    memmove(ptr(offset), src, length);
}

Blob & Blob::operator=(const Blob & blob)
{
    m_impl = blob.m_impl.get() ? blob.m_impl->clone() : nullptr;
    return *this;
}

Blob & Blob::operator=(Blob && blob)
{
    m_impl = std::move(blob.m_impl);
    return *this;
}

}

