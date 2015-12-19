#pragma once

#include <functional>

namespace deliberation
{

namespace detail
{

class BufferImpl;

struct BufferBinding
{
	std::shared_ptr<BufferImpl> buffer;
	bool ranged;
	unsigned int first;
	unsigned int count;
	unsigned int divisor;
};

}

}

