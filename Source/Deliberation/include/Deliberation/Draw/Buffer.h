#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>

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

class DrawContext;
class LayoutedBlob;

class DELIBERATION_API Buffer final
{
public:
    Buffer();

    unsigned int count() const;
    unsigned int size() const;
    const DataLayout & layout() const;

    BufferUpload createUpload(const Blob & data);
    BufferUpload createRawUpload(const Blob & data, unsigned int count);

    void scheduleUpload(const Blob & data);
    void scheduleRawUpload(const Blob & data, unsigned int count);

    void scheduleUpload(const LayoutedBlob & data);

    std::string toString() const;

private:
    friend class BufferUploadExecution;
    friend class DrawContext;
    friend class Draw;
    friend class GLVertexAttributeBinder;

private:
    Buffer(const std::shared_ptr<detail::BufferImpl> & impl);

private:
    std::shared_ptr<detail::BufferImpl> m_impl;
};

}



