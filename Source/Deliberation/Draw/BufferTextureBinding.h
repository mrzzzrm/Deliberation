#pragma once

#include <memory>

#include <Deliberation/Core/IntTypes.h>

namespace deliberation
{

class Buffer;

class BufferTextureBinding final
{
public:
    BufferTextureBinding(const std::shared_ptr<DrawImpl> & drawImpl, u32 index):
        m_drawImpl(drawImpl), m_index(index)
    {}

    void setBuffer(const Buffer & buffer);
    void setBufferRange(const Buffer & buffer, u32 begin, u32 count);

private:
    std::shared_ptr<DrawImpl> m_drawImpl;
    u32 m_index;
};

}