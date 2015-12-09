#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/Blob.h>

#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>

/*
    TODO
        Create buffers with immutable size only?
*/

namespace deliberation
{

namespace detail
{
    class BufferImpl;
}

class Context;

class Buffer final
{
public:
    Buffer();

    unsigned int count() const;
    const BufferLayout & layout() const;

    BufferUpload createUpload(const Blob & data);
    BufferUpload createRawUpload(const Blob & data, unsigned int count);

    std::string toString() const;

private:
    friend class Draw;
    friend class GLVertexAttributeBinder;
    friend class BufferUploadExecution;
    friend class Context;

private:
    Buffer(Context & context, const BufferLayout & layout);

private:
    std::shared_ptr<detail::BufferImpl> m_impl;
};

}



