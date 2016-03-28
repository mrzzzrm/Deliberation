#pragma once

#include <array>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/SystemBase.h>
#include <Deliberation/ECS/EntityData.h>

namespace deliberation
{

class World final
{
public:
    World();
    ~World();

    detail::EntityData & entityData(Entity::id_t id);

    Entity createEntity(const std::string & name = "Entity",
                        Entity::id_t parent = Entity::INVALID_ID);

    template<typename T, typename ... Args>
    T & addSystem(Args &&... args);

    void update();

    std::string toString() const;

private:
    friend class Entity;

private:
    bool isValid(Entity::id_t id) const;
    bool isActive(Entity::id_t id) const;
    void activate(Entity::id_t id);
    void deactivate(Entity::id_t id);
    void remove(Entity::id_t id);
    ComponentBase * component(Entity::id_t id, TypeID::value_t index);
    const ComponentBase * component(Entity::id_t id, TypeID::value_t index) const;
    void addComponent(Entity::id_t id, TypeID::value_t index, ComponentBase * component);
    void removeComponent(Entity::id_t id, TypeID::value_t index);

    void scheduleTransition(Entity::id_t id, detail::EntityTransition transition);
    void propagateTransition(Entity::id_t id, detail::EntityTransition transition);
    void insertChildrenTransitions(Entity::id_t id, detail::EntityTransition transition);

    void performActivation(detail::EntityData & entity);
    void performDeactivation(detail::EntityData & entity);
    void performRemoval(detail::EntityData & entity);

private:
    LinearMap<detail::EntityData>               m_entities;
    std::stack<std::size_t>                     m_entityPool;
    LinearMap<LinearMap<
        std::unique_ptr<ComponentBase>>>        m_components;
    std::vector<std::unique_ptr<SystemBase>>    m_systems;

    std::array<
        std::vector<Entity::id_t>,
        detail::__EntityTransition_Count__>    m_scheduledTransitions;
    std::array<
        std::vector<Entity::id_t>,
        detail::__EntityTransition_Count__>    m_propagatedTransitions;
    std::array<
        std::vector<Entity::id_t>,
        detail::__EntityTransition_Count__>    m_orderedTransitions;
};

}

#include <Deliberation/ECS/World.inl>

