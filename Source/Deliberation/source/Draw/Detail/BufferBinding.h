#pragma once

#include <functional>

namespace deliberation
{

class Buffer;
	
namespace detail
{
	
struct BufferBinding
{
	std::reference_wrapper<const Buffer> buffer;
	bool ranged;
	unsigned int first;
	unsigned int count;
	unsigned int divisor;
};

}

}

