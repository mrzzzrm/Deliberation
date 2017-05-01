#pragma once

#include <memory>
#include <vector>



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

enum class BufferMapping
{
    ReadOnly,
    WriteOnly,
    ReadWrite
};

class Buffer final
{
public:
    Buffer();

    size_t count() const;
    size_t size() const;
    const DataLayout & layout() const;

    void scheduleUpload(const Blob & data);
    void scheduleRawUpload(const Blob & data, size_t count);
    void scheduleUpload(const LayoutedBlob & data);

    void reinit(size_t count);

    LayoutedBlob & map(BufferMapping flags);
    void unmap();

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



