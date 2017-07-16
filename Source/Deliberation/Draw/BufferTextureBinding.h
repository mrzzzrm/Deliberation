#pragma once

#include <glbinding/gl/gl.h>

#include <memory>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{
class Buffer;
class BufferTextureBindingImpl;
class DrawImpl;

class BufferTextureBinding final
{
public:
    BufferTextureBinding() = default;
    BufferTextureBinding(const std::weak_ptr<DrawImpl> drawImpl, u32 index)
        : m_drawImpl(drawImpl), m_index(index)
    {
    }

    void setBuffer(const Buffer & buffer);
    void setBufferRange(const Buffer & buffer, u32 begin, u32 count);

private:
    gl::GLenum internalFormatFromBuffer(const Buffer & buffer) const;

private:
    std::weak_ptr<DrawImpl> m_drawImpl;
    u32                     m_index = 0;
};
}