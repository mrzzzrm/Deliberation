#include <Deliberation/Draw/BufferUpload.h>

#include <Deliberation/Draw/Context.h>

namespace deliberation
{

Buffer & BufferUpload::buffer() const
{
    return m_buffer;
}

const Blob & BufferUpload::data() const
{
    return m_blob;
}

unsigned int BufferUpload::count() const
{
    return m_count;
}

void BufferUpload::schedule()
{
    m_context.scheduleBufferUpload(*this);
}

}

