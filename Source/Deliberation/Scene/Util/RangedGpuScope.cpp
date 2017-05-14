#include <Deliberation/Scene/Util/RangedGpuScope.h>

#define DELIBERATION_NV_TOOLS_EXT 0

#if DELIBERATION_NV_TOOLS_EXT
#include <nvToolsExt.h>
#endif

namespace deliberation
{

#if DELIBERATION_NV_TOOLS_EXT

RangedGpuScope::RangedGpuScope(const std::string & name)
{
    nvtxRangePushA(name.c_str());
}

RangedGpuScope::~RangedGpuScope()
{
    nvtxRangePop();
}

#else

RangedGpuScope::RangedGpuScope(const std::string & name)
{
}

RangedGpuScope::~RangedGpuScope()
{
}

#endif

}