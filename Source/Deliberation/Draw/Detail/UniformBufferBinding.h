#pragma once

#include <memory>

namespace deliberation
{

namespace detail
{

class BufferImpl;

struct UniformBufferBinding
{
	std::shared_ptr<BufferImpl> buffer;
	unsigned int begin;
};

}

}

