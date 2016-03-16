#pragma once

#include <bitset>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

namespace detail
{

enum EntityTransition
{
    EntityTransition_None = 0,
    EntityTransition_Activate,
    EntityTransition_Deactivate,
    EntityTransition_Remove,

    __EntityTransition_Count__
};

enum EntityState
{
    EntityState_None,
    EntityState_Activated,
    EntityState_Deactivated
};

struct EntityData
{
    EntityData();
    EntityData(Entity::id_t id, const std::string & name, Entity::id_t parent);

    Entity::id_t                        id;

    std::string                         name;
    Entity::id_t                        parent;
    std::vector<Entity::id_t>           children;

    EntityState                         state;
    EntityTransition                    transition;

    /*
        Marks in which collection in World the Entity is
        enqueued
    */
    std::bitset<
        __EntityTransition_Count__>     scheduledTransitions;
    std::bitset<
        __EntityTransition_Count__>     propagatedTransitions;
    std::bitset<
        __EntityTransition_Count__>     orderedTransitions;
};

}

}

#include <Deliberation/ECS/Detail/EntityData.inl>
