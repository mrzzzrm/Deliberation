#pragma once

#include <limits>
#include <stdint.h>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

namespace detail
{
    class EntityData;
}

class ComponentBase;
class World;

class Entity final
{
public:
    typedef uint64_t id_t;

public:
    static constexpr id_t INVALID_ID = std::numeric_limits<id_t>::max();

public:
    Entity();

    id_t id() const;

    bool isValid();
    bool isActive();

    const std::string & name() const;

    Entity::id_t parent() const;
    std::vector<Entity::id_t> & children() const;

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

    void activate();
    void deactivate();
    void remove();

    Entity createChild(const std::string & name = "Entity");

private:
    friend class World;

private:
    Entity(World & world, id_t id);

    detail::EntityData & data() const;

    bool hasComponent(TypeID::value_t c);
    ComponentBase * component(TypeID::value_t c);
    const ComponentBase * component(TypeID::value_t c) const;
    void addComponent(TypeID::value_t c, ComponentBase * ptr);
    void removeComponent(TypeID::value_t c);

private:
    World *         m_world;
    id_t            m_id;
};

}

#include <Deliberation/ECS/Entity.inl>

