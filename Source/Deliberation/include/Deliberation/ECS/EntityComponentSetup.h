#pragma once

#include <cstdlib>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/ECS/Defines.h>

namespace deliberation
{

struct DELIBERATION_API EntityComponentSetup
{
    std::vector<std::size_t> componentIndices;
    std::vector<std::size_t> systemIndices;
    SystemBitset             systemBits;
};

}

