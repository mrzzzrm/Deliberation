#include <Deliberation/Draw/UniformBufferHandle.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Detail/DrawImpl.h>

namespace deliberation
{

UniformBufferHandle::UniformBufferHandle(const std::shared_ptr<DrawImpl> &drawImpl,
                                         u32 index) :
    m_drawImpl(drawImpl),
    m_index(index) {

}

void UniformBufferHandle::setBuffer(const Buffer &buffer, u32 begin) {
    Assert((bool) m_drawImpl, "UniformBufferHandle is hollow");

    UniformBufferBinding binding{buffer.m_impl, begin};

    m_drawImpl->uniformBuffers[m_index].reset(binding);
}

}
