#pragma once

#include <array>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/Defines.h>
#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/EntityComponentSetup.h>
#include <Deliberation/ECS/EntityData.h>
#include <Deliberation/ECS/EventManager.h>
#include <Deliberation/ECS/SystemBase.h>

namespace deliberation
{

class World final
{
public:
    World();
    ~World();

    EventManager & eventManager();

    EntityData & entityData(entity_id_t id);

    Entity createEntity(const std::string & name = "Entity",
                        entity_id_t parent = ECS_INVALID_ENTITY_ID);

    template<typename T, typename ... Args>
    T & addSystem(Args &&... args);

    void update(float seconds);

    std::string toString() const;

private:
    friend class Entity;

private:
    bool isValid(entity_id_t id) const;
    void remove(entity_id_t id);
    std::shared_ptr<ComponentBase> component(entity_id_t id, TypeID::value_t index);
    std::shared_ptr<const ComponentBase> component(entity_id_t id, TypeID::value_t index) const;
    void addComponent(entity_id_t id, TypeID::value_t index, std::unique_ptr<ComponentBase> component);
    void removeComponent(entity_id_t id, TypeID::value_t index);

    std::size_t entityIndex(entity_id_t id) const;
    EntityComponentSetup * componentSetup(const ComponentBitset & componentBits);

private:
    SparseVector<EntityData>                    m_entities;
    std::unordered_map<entity_id_t,
        std::size_t>                            m_entityIndexByID;
    entity_id_t                                 m_entityIDCounter;

    std::unordered_map<ComponentBitset,
        EntityComponentSetup>                   m_entityComponentSetups;

    LinearMap<LinearMap<
        std::shared_ptr<ComponentBase>>>        m_components;

    LinearMap<std::unique_ptr<SystemBase>>      m_systems;

    EventManager                                m_eventManager;
};

}

#include <Deliberation/ECS/World.inl>

