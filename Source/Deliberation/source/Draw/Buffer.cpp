#include <Deliberation/Draw/Buffer.h>

#include "Detail/BufferImpl.h"

namespace deliberation
{

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

}

