#pragma once

#include <unordered_map>
#include <unordered_set>

#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/ECS/ComponentFilter.h>

namespace deliberation
{
class EventListenerProxy;
class World;

class SystemBase
{
public:
    SystemBase(World & world, const ComponentFilter & filter);

    World &                 world();
    const ComponentFilter & filter() const;

    virtual std::size_t index() const = 0;
    virtual std::string name() const = 0;

    bool accepts(const Entity & entity);

    void addEntity(Entity & entity);
    void removeEntity(Entity & entity);

    void frameBegin();
    void beforeGameUpdate();
    void gameUpdate(float seconds);
    void frameUpdate(float seconds);
    void prePhysicsUpdate(float seconds);
    void postPhysicsUpdate(float seconds);
    void frameComplete(float seconds);

    virtual void onCreated() {}
    virtual void onRemoved() {}

    virtual void renderImGui() const {}

protected:
    virtual void onEntityAdded(Entity & entity);
    virtual void onEntityRemoved(Entity & entity);

    virtual void onEntityGameUpdate(Entity & entity, float seconds) {}
    virtual void
    onEntityPrePhysicsUpdate(Entity & entity, float physicsTimestep);
    virtual void
    onEntityPostPhysicsUpdate(Entity & entity, float physicsTimestep)
    {
    }
    virtual void onFrameBegin();
    virtual void onGameUpdate(float seconds) {}
    virtual void onFrameUpdate(float seconds) {}
    virtual void onPrePhysicsUpdate(float seconds);
    virtual void onPostPhysicsUpdate(float seconds) {}
    virtual void onFrameComplete(float seconds);

protected:
    struct EntityEntry
    {
        EntityId id;
        bool     active;
    };

protected:
    World &                   m_world;
    SparseVector<EntityEntry> m_entities;

private:
    ComponentFilter                           m_filter;
    std::unordered_map<EntityId, std::size_t> m_entityIndexByID;
};
};
