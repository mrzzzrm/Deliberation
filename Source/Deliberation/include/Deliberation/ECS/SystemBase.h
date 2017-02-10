#pragma once

#include <unordered_set>
#include <unordered_map>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/ECS/ComponentFilter.h>

namespace deliberation
{

class World;

class DELIBERATION_API SystemBase
{
public:
    SystemBase(World & world, const ComponentFilter & filter);
    virtual ~SystemBase();

    World & world();
    const ComponentFilter & filter() const;

    virtual std::size_t index() const = 0;

    bool accepts(const Entity & entity);

    void addEntity(Entity & entity);
    void removeEntity(Entity & entity);

    void beforeUpdate();
    void update(float seconds);
    void prePhysicsUpdate(float seconds);

protected:
    virtual void onEntityAdded(Entity & entity);
    virtual void onEntityRemoved(Entity & entity);
    virtual void onUpdate(Entity & entity, float seconds);
    virtual void onPrePhysicsUpdate(Entity & entity, float physicsTimestep);

private:
    struct EntityEntry
    {
        entity_id_t id;
        bool        active;
    };

private:
    World &                          m_world;
    ComponentFilter                  m_filter;
    SparseVector<EntityEntry>        m_entities;
    std::unordered_map<entity_id_t,
        std::size_t>                 m_entityIndexByID;
};

};

