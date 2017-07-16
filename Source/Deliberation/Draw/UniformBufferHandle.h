#pragma once

#include <memory>

#include <Deliberation/Draw/Detail/DrawImpl.h>
#include <Deliberation/Draw/ProgramInterfaceUniformBlock.h>

namespace deliberation
{
class Buffer;

class UniformBufferHandle final
{
public:
    UniformBufferHandle() = default;
    UniformBufferHandle(const std::shared_ptr<DrawImpl> & drawImpl, u32 index);

    void setBuffer(const Buffer & buffer, u32 begin = 0);

private:
    std::shared_ptr<DrawImpl> m_drawImpl;
    u32                       m_index = 0;
};
}