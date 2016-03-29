#pragma once

#include <bitset>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/ECS/Defines.h>

namespace deliberation
{

class EntityComponentSetup;

struct DELIBERATION_API EntityData
{
    EntityData();
    EntityData(entity_id_t id, const std::string & name, entity_id_t parent);

    entity_id_t                 id;
    std::string                 name;
    entity_id_t                 parent;
    std::vector<entity_id_t>    children;

    ComponentBitset             componentBits;

    EntityComponentSetup *      componentSetup;
};

}

#include <Deliberation/ECS/EntityData.inl>
