#pragma once

#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/ActivityComponent.h>

namespace deliberation
{

class ActivityComponentPrototype:
    public ComponentPrototype<ActivityComponent>
{
public:
    void updateComponent(const Entity & entity, ActivityComponent & activityComponent) override;
};

}