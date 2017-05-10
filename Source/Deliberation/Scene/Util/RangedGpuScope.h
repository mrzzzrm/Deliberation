#pragma once

#include <string>

namespace deliberation
{

class RangedGpuScope final
{
public:
    RangedGpuScope(const std::string & name);
    ~RangedGpuScope();
};

}
