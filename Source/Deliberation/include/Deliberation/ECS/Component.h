#pragma once

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/ECS/ComponentBase.h>

namespace deliberation
{

friend class World;

template<typename T>
class Component:
    public ComponentBase
{
public:
    Entity entity();

protected:
    virtual void onCreate();

private:
    friend class World;

private:
    Component(Entity entity);
};

}

