#include <Deliberation/Scene/Util/RangedGpuScope.h>

#include <nvToolsExt.h>

namespace deliberation
{

RangedGpuScope::RangedGpuScope(const std::string & name)
{
    nvtxRangePushA(name.c_str());
}

RangedGpuScope::~RangedGpuScope()
{
    nvtxRangePop();
}

}