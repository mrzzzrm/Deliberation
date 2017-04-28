#pragma once

#include <array>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>



#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/AbstractWorld.h>
#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/Defines.h>
#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/EntityComponentSetup.h>
#include <Deliberation/ECS/EntityData.h>
#include <Deliberation/ECS/EventManager.h>
#include <Deliberation/ECS/SystemBase.h>
#include <Deliberation/ECS/WorldProfiler.h>

namespace deliberation
{

class World final:
    public AbstractWorld
{
public:
    World();
    ~World();

    EventManager & eventManager();
    const WorldProfiler & profiler() const;

    EntityData & entityData(EntityId id);
    Entity entity(EntityId id);
    Entity entityByIndex(size_t index);
    Entity entityById(EntityId id);

    const SparseVector<EntityData> & entities() const { return m_entities; }

    Entity createEntity(const std::string & name = "Entity",
                        EntityId parent = ECS_INVALID_ENTITY_ID);

    template<typename T, typename ... Args>
    std::shared_ptr<T> addSystem(Args &&... args);

    void frameBegin();
    void update(float seconds);
    void prePhysicsUpdate(float seconds);
    void render();

    void frameComplete();

    std::string toString() const;

    template<typename T>
    T & system();

    /**
     * From AbstractWorld
     */
    void emit(size_t entityIndex, TypeID::value_t eventType, const void * event) final override;


private:
    friend class Entity;

private:
    struct ComponentRemoval
    {
        ComponentRemoval(EntityId entityId, ComponentTypeId componentTypeId):
            entityId(entityId),
            componentTypeId(componentTypeId)
        {}

        EntityId        entityId;
        ComponentTypeId componentTypeId;
    };

private:
    bool isValid(EntityId id) const;
    void scheduleEntityRemoval(EntityId id);
    std::shared_ptr<ComponentBase> component(EntityId id, TypeID::value_t index);
    std::shared_ptr<const ComponentBase> component(EntityId id, TypeID::value_t index) const;
    void addComponent(EntityId id, TypeID::value_t index, std::shared_ptr<ComponentBase> component);
    void scheduleComponentRemoval(EntityId id, TypeID::value_t index);
    void removeComponent(const ComponentRemoval & componentRemoval);
    void removeEntity(EntityId entityId);

    std::size_t entityIndex(EntityId id) const;
    EntityComponentSetup * componentSetup(const ComponentBitset & componentBits);

private:
    SparseVector<EntityData>                    m_entities;
    std::unordered_map<EntityId,
        std::size_t>                            m_entityIndexByID;
    EntityId                                    m_entityIDCounter;

    std::unordered_map<ComponentBitset,
        EntityComponentSetup>                   m_entityComponentSetups;

    LinearMap<LinearMap<
        std::shared_ptr<ComponentBase>>>        m_components;

    std::vector<EntityId>                       m_entityRemovals;
    std::vector<ComponentRemoval>               m_componentRemovals;

    LinearMap<std::shared_ptr<SystemBase>>      m_systems;

    EventManager                                m_eventManager;

    WorldProfiler                               m_profiler;
};

}

#include <Deliberation/ECS/World.inl>

