#pragma once

#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/ECS/Entity.h>

namespace deliberation
{

namespace detail
{

struct EntityData
{
    EntityData(Entity::id_t id, const std::string & name, Entity::id_t parent);

    Entity::id_t                id;
    std::string                 name;
    Entity::id_t                parent;
    std::vector<Entity::id_t>   children;
    bool                        active;
    bool                        activationScheduled;
    bool                        deactivationScheduled;
    bool                        removalScheduled;
};

}

}

#include "EntityData.inl"
