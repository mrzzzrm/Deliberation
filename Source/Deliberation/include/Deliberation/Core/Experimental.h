#pragma once

namespace deliberation
{

namespace optional_ns_wrapper
{
#include "optional.hpp"	
}

namespace variant_ns_wrapper
{
#include "variant"
}

using optional_ns = optional_ns_wrapper::std::experimental;
using variant_ns = variant_ns_wrapper::std::experimental;

}