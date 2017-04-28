#pragma once

#include <cstdlib>


#include <Deliberation/Core/TypeID.h>
#include <Deliberation/ECS/Defines.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

struct EntityComponentSetup
{
    std::vector<ComponentTypeId>    componentTypeIds;
    std::vector<std::size_t>        systemIndices;
    SystemBitset                    systemBits;
    std::unordered_map<TypeID::value_t, std::vector<size_t>>
                                    componentIndicesByEventType;
};

}

