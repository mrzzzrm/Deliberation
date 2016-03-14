#include <Deliberation/ECS/Entity.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

Entity::Entity():
    m_id(INVALID_ID)
{

}

id_t Entity::id() const
{
    return m_id;
}

bool Entity::isValid()
{
    return m_world.isValid(m_id);
}

bool Entity::isActive()
{
    return m_world.isActive(m_id);
}

const std::string & Entity::name() const
{
    return data().name;
}

Entity Entity::parent() const
{
    return data().parent;
}

std::vector<Entity> & Entity::children() const
{
    return data().children;
}

void Entity::activate()
{
    m_world.activate(m_id);
}

void Entity::deactivate()
{
    m_world.deactivate(m_id);
}

void Entity::remove()
{
    m_world.remove(m_id);
}

Entity::Entity(World & world, id_t id):
    m_world(world),
    m_id(id)
{

}

EntityData & Entity::data() const
{
    return m_world.entityData(m_id)
}

}

