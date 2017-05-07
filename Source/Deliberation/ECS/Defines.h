#pragma once

#include <bitset>
#include <limits>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{
const auto ECS_MAX_NUM_COMPONENTS = 64;
const auto ECS_MAX_NUM_SYSTEMS = 128;

typedef std::bitset<ECS_MAX_NUM_COMPONENTS> ComponentBitset;
typedef std::bitset<ECS_MAX_NUM_SYSTEMS>    SystemBitset;
typedef uint64_t                            EntityId;
typedef TypeID::value_t                     ComponentTypeId;
typedef TypeID::value_t                     SystemTypeId;

const auto ECS_INVALID_ENTITY_ID = std::numeric_limits<EntityId>::max();

struct ComponentEventFamily
{
};
}
