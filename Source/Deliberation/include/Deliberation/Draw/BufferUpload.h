#pragma once

#include <vector>

#include <Deliberation/Core/Blob.h>

namespace deliberation
{

class Buffer;
class Context;

class BufferUpload final
{
public:
    /*
        TODO
            Context takes the template work, it could also generate the Blob and pass
            it to BufferUpload, so BufferUpload doesn't need another set of template-
            constructors
    */
    template<typename T>
    BufferUpload(Context & context, Buffer & buffer, const std::vector<T> & data);

    template<typename T>
    BufferUpload(Context & context, Buffer & buffer, std::vector<T> && data);

    template<typename T>
    BufferUpload(Context & context, Buffer & buffer, const std::vector<T> & data, unsigned int count);

    template<typename T>
    BufferUpload(Context & context, Buffer & buffer, std::vector<T> && data, unsigned int count);

    Buffer & buffer() const;
    const Blob & data() const;
    unsigned int count() const;

    void schedule();

private:
    Context & m_context;
    Buffer & m_buffer;
    Blob m_blob;
    unsigned int m_count;
};

}

#include <Deliberation/Draw/BufferUpload.inl>
