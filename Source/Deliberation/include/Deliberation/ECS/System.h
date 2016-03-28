#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/ECS/SystemBase.h>

namespace deliberation
{

template<typename T>
class DELIBERATION_API System:
    public SystemBase
{
public:
    System(World & world, const ComponentFilter & filter);


private:

};

}

