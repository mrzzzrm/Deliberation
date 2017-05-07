#pragma once

#include <memory>

namespace deliberation
{
class BufferImpl;

struct UniformBufferBinding
{
    std::shared_ptr<BufferImpl> buffer;
    unsigned int                begin;
};
}
