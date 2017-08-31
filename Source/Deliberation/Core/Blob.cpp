#include <Deliberation/Core/Blob.h>

#include <sstream>
#include <string.h>

namespace deliberation
{
Blob Blob::fromRawData(const void * src, size_t size)
{
    auto blob = Blob(size);
    blob.write(0, src, size);
    return blob;
}

Blob::Blob() : m_impl(nullptr) {}

Blob::Blob(std::size_t size)
    : m_impl(std::make_unique<detail::VectorBlobImpl<char>>(size))
{
}

Blob::Blob(std::unique_ptr<detail::IBlobImpl> && impl) : m_impl(std::move(impl))
{
}

Blob::Blob(const Blob & blob)
    : m_impl(blob.m_impl ? blob.m_impl->clone() : nullptr)
{
}

Blob::Blob(Blob && blob) : m_impl(blob.m_impl ? blob.m_impl->clone() : nullptr)
{
}

std::size_t Blob::size() const { return m_impl ? m_impl->size() : 0u; }

void * Blob::ptr(size_t offset)
{
    Assert(offset < size());

    return m_impl ? ((char *)m_impl->ptr()) + offset : nullptr;
}

const void * Blob::ptr(size_t offset) const
{
    Assert(offset < size());

    return m_impl ? ((char *)m_impl->ptr()) + offset : nullptr;
}

void * Blob::ptrRaw(size_t offset)
{
    return m_impl ? ((char *)m_impl->ptr()) + offset : nullptr;
}

const void * Blob::ptrRaw(size_t offset) const
{
    return m_impl ? ((char *)m_impl->ptr()) + offset : nullptr;
}

void Blob::write(std::size_t offset, const void * src, size_t length)
{
    Assert(offset + length <= size());
    Assert(ptr());

    memmove(ptr(offset), src, length);
}

void Blob::resize(size_t size)
{
    AssertM(m_impl.get(), "Hollow Blob");
    m_impl->resize(size);
}

void Blob::reserve(size_t size)
{
    AssertM(m_impl.get(), "Hollow Blob");
    m_impl->reserve(size);
}

std::string Blob::toString() const
{
    std::stringstream stream;
    stream << "Blob: {";

    for (auto b = 0; b < size(); b++)
    {
        stream << std::hex << (int)access<unsigned char>(b) << ", ";
    }

    stream << "}";
    return stream.str();
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
