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

const void * Blob::ptr() const
{
    return m_impl ? m_impl->ptr() : nullptr;
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

