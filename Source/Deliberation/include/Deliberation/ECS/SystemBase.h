#pragma once

#include <unordered_set>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/ECS/ComponentFilter.h>

namespace deliberation
{

class World;

class SystemBase
{
public:
    SystemBase(World & world, const ComponentFilter & filter);
    virtual ~SystemBase();

    World & world();

    bool accepts(const Entity & entity);

    void addEntity(Entity & entity);
    void removeEntity(Entity & entity);

    void update(float seconds);

protected:
    virtual void onEntityAdded(Entity & entity);
    virtual void onEntityRemoved(Entity & entity);
    virtual void onUpdate(Entity & entity, float seconds);

private:
    World &                          m_world;
    ComponentFilter                  m_filter;
    std::unordered_set<Entity::id_t> m_entities;
};

};

