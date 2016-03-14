#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include <vector>
#include <stack>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/LinearMap.h>
#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/Detail/EntityData.h>

namespace deliberation
{

class World final
{
public:
    World();

    Entity createEntity(const std::string & name = "Nameless Entity");

    void update();

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
    void addComponent(Entity::id_t id, TypeID::value_t index, ComponentBase * component) const;
    void removeComponent(Entity::id_t id, TypeID::value_t index);

private:
    LinearMap<detail::EntityData>               m_entities;
    std::stack<std::size_t>                     m_entityPool;
    LinearMap<LinearMap<
        std::unique_ptr<ComponentBase>>         m_components;

    std::queue<Entity::id_t>                    m_activationQueue;
    std::queue<Entity::id_t>                    m_deactivationQueue;
    std::queue<Entity::id_t>                    m_removalQueue;
}

}

