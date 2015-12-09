#include <Deliberation/Draw/Buffer.h>

#include "Detail/BufferImpl.h"

namespace deliberation
{

Buffer::Buffer():
    m_impl(nullptr)
{

}

const BufferLayout & Buffer::layout() const
{
    return m_impl->layout;
}

unsigned int Buffer::count() const
{
    return m_impl->count;
}

std::string Buffer::toString() const
{
    return
        "Layout: " + layout().toString() + "\n"
        "Count: " + std::to_string(count());
}

Buffer::Buffer(Context & context, const BufferLayout & layout)
{
    m_impl = std::make_shared<detail::BufferImpl>(context);
    m_impl->layout = layout;
}

BufferUpload Buffer::createUpload(const Blob & data)
{
    return BufferUpload(m_impl->context, *this, data);
}

BufferUpload Buffer::createRawUpload(const Blob & data, unsigned int count)
{
    return BufferUpload(m_impl->context, *this, data, count);
}

}

