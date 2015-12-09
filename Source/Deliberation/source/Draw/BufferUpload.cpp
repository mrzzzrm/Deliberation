#include <Deliberation/Draw/BufferUpload.h>

#include <string>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Context.h>

namespace deliberation
{

BufferUpload::BufferUpload(Context & context,
                           Buffer & buffer,
                           const Blob & data):
    m_context(context),
    m_buffer(buffer),
    m_blob(data),
    m_count(data.size() / buffer.layout().stride())
{
    Assert(data.size() % buffer.layout().stride() == 0, "");
}

BufferUpload::BufferUpload(Context & context,
                           Buffer & buffer,
                           const Blob & data,
                           unsigned int count):
    m_context(context),
    m_buffer(buffer),
    m_blob(data),
    m_count(count)
{
    /*
        TODO
            This Assert is broken
    */
//    Assert(sizeof(T) == m_buffer.layout().stride(),
//           "BufferLayout stride " + std::to_string(m_buffer.layout().stride()) +
//           " differs from data size " + std::to_string(sizeof(T)));
}

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

