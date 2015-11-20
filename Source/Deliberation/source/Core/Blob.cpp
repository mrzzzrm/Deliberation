#include <Deliberation/Core/Blob.h>

namespace deliberation
{

Blob::Blob(const Blob & blob):
    m_impl(blob.m_impl->clone())
{
}

Blob::Blob(Blob && blob):
    m_impl(blob.m_impl->clone())
{

}

std::size_t Blob::size() const
{
    return m_impl->size();
}

const void * Blob::ptr() const
{
    return m_impl->ptr();
}

Blob & Blob::operator=(Blob && blob)
{
    m_impl = std::move(blob.m_impl);
    return *this;
}

}

