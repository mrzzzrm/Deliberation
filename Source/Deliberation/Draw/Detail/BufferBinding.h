#pragma once

#include <functional>

namespace deliberation
{

class BufferImpl;

struct BufferBinding
{
	std::shared_ptr<BufferImpl> buffer;
	bool ranged = false;
	unsigned int first = 0;
	unsigned int count = 0;
	unsigned int divisor = 1;
};

}

