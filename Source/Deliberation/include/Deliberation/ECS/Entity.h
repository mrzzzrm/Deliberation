#pragma once

#include <limits>
#include <stdint.h>
#include <string>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class EntityData;
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

    Entity parent() const;
    std::vector<Entity> & children() const;

    template<typename T>
    bool hasComponent() const;

    template<typename T>
    T & component();

    template<typename T>
    const T & component() const;

    template<typename T, typename ... Args>
    T & addComponent(Args ...&& args);

    template<typename T>
    void removeComponent();

    void activate();
    void deactivate();
    void remove();

private:
    friend class World;

private:
    Entity(World & world, id_t id);

    EntityData & data() const;

private:
    mutable World & m_world;
    id_t            m_id;
};

}

#include <Deliberation/ECS/Entity.inl>

