#pragma once

#include <array>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/Core/EventDomain.h>

#include <Deliberation/ECS/AbstractWorld.h>
#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/Defines.h>
#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/EntityComponentSetup.h>
#include <Deliberation/ECS/EntityData.h>
#include <Deliberation/ECS/SystemBase.h>
#include <Deliberation/ECS/WorldProfiler.h>

namespace deliberation
{
class ActivityManager;
class UpdateFrame;

class World final : public AbstractWorld
{
public:
    World();
    ~World() override;

    const WorldProfiler & profiler() const;
    const std::shared_ptr<ActivityManager> & activityManager() { return m_activityManager; }

    EntityData & entityData(EntityId id);
    Entity       entity(EntityId id);
    Entity       entityByIndex(size_t index);
    Entity       entityById(EntityId id);

    const SparseVector<EntityData> & entities() const { return m_entities; }

    Entity createEntity(
        const std::string & name = "Entity",
        EntityId            parent = ECS_INVALID_ENTITY_ID);

    template<typename T, typename... Args>
    std::shared_ptr<T> addSystem(Args &&... args);

    /**
     * Predefined phases of a frame
     */
    // @{
    void frameBeginPhase();
    void gameUpdatePhase(const UpdateFrame & updateFrame);
    void frameUpdatePhase(const UpdateFrame & updateFrame);
    void prePhysicsUpdatePhase(const UpdateFrame & updateFrame);
    void postPhysicsUpdatePhase(const UpdateFrame & updateFrame);
    void frameCompletePhase(const UpdateFrame & updateFrame);
    // @}

    std::string toString() const;

    template<typename T>
    std::shared_ptr<T> system();

    template<typename T>
    T & systemRef();

    const LinearMap<std::shared_ptr<SystemBase>> systems() const
    {
        return m_systems;
    }

    /**
     * Internals, use with care
     */
    std::shared_ptr<ComponentBase>
    component(EntityId id, TypeID::value_t index);
    std::shared_ptr<const ComponentBase>
    component(EntityId id, TypeID::value_t index) const;

private:
    friend class Entity;

private:
    struct ComponentRemoval
    {
        ComponentRemoval(EntityId entityId, ComponentTypeId componentTypeId)
            : entityId(entityId), componentTypeId(componentTypeId)
        {
        }

        EntityId        entityId;
        ComponentTypeId componentTypeId;
    };

private:
    bool isValid(EntityId id) const;
    void scheduleEntityRemoval(EntityId id);
    void addComponent(
        EntityId                       id,
        TypeID::value_t                index,
        std::shared_ptr<ComponentBase> component);
    void scheduleComponentRemoval(EntityId id, TypeID::value_t index);
    void removeComponent(const ComponentRemoval & componentRemoval);
    void removeEntity(EntityId entityId);

    std::size_t entityIndex(EntityId id) const;
    EntityComponentSetup *
    componentSetup(const ComponentBitset & componentBits);

private:
    std::shared_ptr<ActivityManager>          m_activityManager;

    SparseVector<EntityData>                  m_entities;
    std::unordered_map<EntityId, std::size_t> m_entityIndexByID;
    EntityId                                  m_entityIDCounter;

    std::unordered_map<ComponentBitset, EntityComponentSetup>
        m_entityComponentSetups;

    LinearMap<LinearMap<std::shared_ptr<ComponentBase>>> m_components;

    std::vector<EntityId>         m_entityRemovals;
    std::vector<ComponentRemoval> m_componentRemovals;

    LinearMap<std::shared_ptr<SystemBase>> m_systems;

    WorldProfiler m_profiler;
};
}

#include <Deliberation/ECS/World.inl>
