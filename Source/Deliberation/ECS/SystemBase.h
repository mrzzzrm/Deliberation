#pragma once

#include <unordered_map>
#include <unordered_set>

#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/ECS/ComponentFilter.h>

namespace deliberation
{
class EventListenerProxy;
class UpdateFrame;
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
    void gameUpdate(const UpdateFrame & updateFrame);
    void frameUpdate(const UpdateFrame & updateFrame);
    void prePhysicsUpdate(const UpdateFrame & updateFrame);
    void postPhysicsUpdate(const UpdateFrame & updateFrame);
    void frameComplete(const UpdateFrame & updateFrame);

    virtual void onCreated() {}
    virtual void onRemoved() {}

    virtual void renderImGui() const {}

protected:
    virtual void onEntityAdded(Entity & entity);
    virtual void onEntityRemoved(Entity & entity);

    virtual void onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame) {}
    virtual void
    onEntityPrePhysicsUpdate(Entity & entity, const UpdateFrame & updateFrame);
    virtual void
    onEntityPostPhysicsUpdate(Entity & entity, const UpdateFrame & updateFrame)
    {
    }
    virtual void onFrameBegin();
    virtual void onGameUpdate(const UpdateFrame & updateFrame) {}
    virtual void onFrameUpdate(const UpdateFrame & updateFrame) {}
    virtual void onPrePhysicsUpdate(const UpdateFrame & updateFrame);
    virtual void onPostPhysicsUpdate(const UpdateFrame & updateFrame) {}
    virtual void onFrameComplete(const UpdateFrame & updateFrame);

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
