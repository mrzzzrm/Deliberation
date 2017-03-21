#pragma once

#include <limits>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Defines.h>

namespace deliberation
{

class ComponentBase;
class EntityData;
class World;

class Entity final
{
public:
    Entity();

    World & world() const;
    entity_id_t id() const;
    const std::string & name() const;
    entity_id_t parent() const;
    const std::vector<entity_id_t> & children() const;
    const ComponentBitset & componentBits() const;

    bool isValid() const;

    template<typename T>
    bool hasComponent() const;

    template<typename T>
    T & component();

    template<typename T>
    const T & component() const;

    template<typename T, typename ... Args>
    T & addComponent(Args &&... args);

    template<typename T>
    void removeComponent();

    void remove();

    Entity createChild(const std::string & name = "Entity");

    std::string toString() const;

    bool operator<(const Entity & rhs) const;
    bool operator==(const Entity & rhs) const;
    bool operator!=(const Entity & rhs) const;

private:
    friend class World;
    friend class SystemBase;

private:
    Entity(World & world, entity_id_t id);

    EntityData & data() const;

    bool hasComponent(TypeID::value_t c) const;
    ComponentBase & component(TypeID::value_t c);
    const ComponentBase & component(TypeID::value_t c) const;
    void addComponent(TypeID::value_t c, std::shared_ptr<ComponentBase> component);
    void removeComponent(TypeID::value_t c);

private:
    World *     m_world;
    entity_id_t m_id;
};

}

#include <Deliberation/ECS/Entity.inl>

