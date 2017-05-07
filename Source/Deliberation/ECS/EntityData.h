#pragma once

#include <bitset>
#include <string>
#include <vector>

#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/ECS/Defines.h>

namespace deliberation
{
class EntityComponentSetup;

enum class EntityPhase
{
    Active,
    ScheduledForRemoval
};

struct EntityData
{
    EntityData();
    EntityData(EntityId id, const std::string & name, EntityId parent);

    EntityId              id;
    std::string           name;
    EntityId              parent;
    std::vector<EntityId> children;

    ComponentBitset componentBits;

    EntityComponentSetup * componentSetup;

    EntityPhase phase = EntityPhase::Active;
};
}

#include <Deliberation/ECS/EntityData.inl>
