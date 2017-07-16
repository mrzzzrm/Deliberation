#pragma once

#include <limits>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

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
    Entity(World & world, EntityId id);
    Entity();

    World &                       world() const;
    EntityId                      id() const;
    const std::string &           name() const;
    EntityId                      parent() const;
    const std::vector<EntityId> & children() const;
    const ComponentBitset &       componentBits() const;

    bool isValid() const;

    template<typename T>
    bool hasComponent() const;

    template<typename T>
    T & component() const;

    template<typename T, typename... Args>
    T & addComponent(Args &&... args);

    template<typename T>
    void scheduleComponentRemoval();

    void scheduleRemoval();

    Entity createChild(const std::string & name = "Entity");

    std::string toString() const;

    bool operator<(const Entity & rhs) const;
    bool operator==(const Entity & rhs) const;
    bool operator!=(const Entity & rhs) const;

private:
    friend class World;
    friend class SystemBase;

private:
    EntityData & data() const;

    bool            hasComponent(TypeID::value_t c) const;
    ComponentBase & component(TypeID::value_t c) const;
    void
         addComponent(TypeID::value_t c, std::shared_ptr<ComponentBase> component);
    void scheduleComponentRemoval(TypeID::value_t c);

private:
    World *  m_world;
    EntityId m_id;
};
}

#include <Deliberation/ECS/Entity.inl>
