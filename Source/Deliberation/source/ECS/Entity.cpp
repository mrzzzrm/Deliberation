#include <Deliberation/ECS/Entity.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/Detail/EntityData.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{

Entity::Entity():
    m_world(nullptr),
    m_id(INVALID_ID)
{

}

Entity::id_t Entity::id() const
{
    return m_id;
}

bool Entity::isValid()
{
    return !m_world || m_world->isValid(m_id);
}

bool Entity::isActive()
{
    return m_world->isActive(m_id);
}

const std::string & Entity::name() const
{
    return data().name;
}

Entity::id_t Entity::parent() const
{
    return data().parent;
}

std::vector<Entity::id_t> & Entity::children() const
{
    return data().children;
}

void Entity::activate()
{
    Assert(m_world, "Entity is hollow");

    m_world->activate(m_id);
}

void Entity::deactivate()
{
    Assert(m_world, "Entity is hollow");

    m_world->deactivate(m_id);
}

void Entity::remove()
{
    Assert(m_world, "Entity is hollow");

    m_world->remove(m_id);
}

Entity::Entity(World & world, id_t id):
    m_world(&world),
    m_id(id)
{

}

detail::EntityData & Entity::data() const
{
    Assert(m_world, "Entity is hollow");

    return m_world->entityData(m_id);
}

bool Entity::hasComponent(TypeID::value_t c)
{
    Assert(m_world, "Entity is hollow");

    return m_world->component(m_id, c) != nullptr;
}

ComponentBase * Entity::component(TypeID::value_t c)
{
    Assert(m_world, "Entity is hollow");

    return m_world->component(m_id, c);
}

const ComponentBase * Entity::component(TypeID::value_t c) const
{
    Assert(m_world, "Entity is hollow");

    return m_world->component(m_id, c);
}

void Entity::addComponent(TypeID::value_t c, ComponentBase * ptr)
{
    Assert(m_world, "Entity is hollow");

    m_world->addComponent(m_id, c, ptr);
}

void Entity::removeComponent(TypeID::value_t c)
{
    Assert(m_world, "Entity is hollow");

    m_world->removeComponent(m_id, c);
}

Entity Entity::createChild(const std::string & name)
{
    Assert(m_world, "Entity is hollow");

    return m_world->createEntity(name, m_id);
}

}

