#pragma once

#include <bitset>

namespace deliberation
{

const auto ECS_MAX_NUM_COMPONENTS = 64;

typedef std::bitset<ECS_MAX_NUM_COMPONENTS> ComponentBitset;

}

