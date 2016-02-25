#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Blob.h>

#include <Deliberation/Draw/BufferLayout.h>
#include <Deliberation/Draw/BufferUpload.h>

/*
    TODO
        Create buffers with immutable size only?
        upload() = createUpload().schedule()
*/

namespace deliberation
{

namespace detail
{
    class BufferImpl;
}

class Context;

class DELIBERATION_API Buffer final
{
public:
    Buffer();

    unsigned int count() const;
    const BufferLayout & layout() const;

    BufferUpload createUpload(const Blob & data);
    BufferUpload createRawUpload(const Blob & data, unsigned int count);

    std::string toString() const;

private:
    friend class BufferUploadExecution;
    friend class Context;
    friend class Draw;
    friend class GLVertexAttributeBinder;

private:
    Buffer(const std::shared_ptr<detail::BufferImpl> & impl);

private:
    std::shared_ptr<detail::BufferImpl> m_impl;
};

}



