#pragma once

#include <memory>
#include <string>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/EntityPrototype.h>

namespace deliberation
{

struct LevelEntity
{
    std::string                         lid;
    Entity                              entity;
    std::shared_ptr<EntityPrototype>    entityPrototype;
};

}