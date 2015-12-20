#pragma once

#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Blob.h>

namespace deliberation
{

class Buffer;
class Context;

class DELIBERATION_API BufferUpload final
{
public:
    BufferUpload(Context & context, Buffer & buffer, const Blob & data);
    BufferUpload(Context & context, Buffer & buffer, const Blob & data, unsigned int count);

    Buffer & buffer() const;
    const Blob & data() const;
    unsigned int count() const;

    void schedule();

private:
    Context &       m_context;
    Buffer &        m_buffer;
    Blob            m_blob;
    unsigned int    m_count;
};

}


