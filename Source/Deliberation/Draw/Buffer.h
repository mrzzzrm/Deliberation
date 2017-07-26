#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/Blob.h>
#include <Deliberation/Core/DataLayout.h>

#include <Deliberation/Draw/AbstractDrawObject.h>
#include <Deliberation/Draw/Enum.h>

namespace deliberation
{
class BufferImpl;
class DrawContext;
class LayoutedBlob;

class Buffer final:
    public AbstractDrawObject<BufferImpl>
{
public:
    Buffer() = default;
    Buffer(const std::shared_ptr<BufferImpl> & impl);

    bool isInitialized() const { return (bool)m_impl; }

    size_t             count() const;
    size_t             size() const;
    const DataLayout & layout() const;

    void upload(const Blob & data);
    void rawUpload(const Blob & data, size_t count);
    void upload(const LayoutedBlob & data);

    void reinit(size_t count);

    void mapped(
        BufferMapping                                       flags,
        const std::function<void(LayoutedBlob & mapping)> & fn);

    std::string toString() const;

private:
    friend class BufferTextureBinding;
    friend class BufferUploadExecution;
    friend class DrawContext;
    friend class Draw;
    friend class GLVertexAttributeBinder;
    friend class UniformBufferHandle;
};
}
