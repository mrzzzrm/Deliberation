#pragma once

#include <bitset>
#include <limits>

namespace deliberation
{

const auto ECS_MAX_NUM_COMPONENTS = 64;
const auto ECS_MAX_NUM_SYSTEMS = 128;

typedef std::bitset<ECS_MAX_NUM_COMPONENTS> ComponentBitset;
typedef std::bitset<ECS_MAX_NUM_SYSTEMS> SystemBitset;
typedef uint64_t entity_id_t;

const auto ECS_INVALID_ENTITY_ID = std::numeric_limits<entity_id_t>::max();

struct ComponentEventFamily {};

}

