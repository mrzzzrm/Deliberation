#include <Deliberation/ECS/Entity.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/EntityData.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{

Entity::Entity():
    m_world(nullptr),
    m_id(ECS_INVALID_ENTITY_ID)
{

}

World & Entity::world() const
{
    Assert(m_world, "");
    return *m_world;
}

entity_id_t Entity::id() const
{
    return m_id;
}

const std::string & Entity::name() const
{
    return data().name;
}

entity_id_t Entity::parent() const
{
    return data().parent;
}

const std::vector<entity_id_t> & Entity::children() const
{
    return data().children;
}

const ComponentBitset & Entity::componentBits() const
{
    return data().componentBits;
}

bool Entity::isValid() const
{
    return !m_world || m_world->isValid(m_id);
}

void Entity::remove()
{
    Assert(m_world, "Entity is hollow");

    m_world->remove(m_id);
}

Entity::Entity(World & world, entity_id_t id):
    m_world(&world),
    m_id(id)
{

}

EntityData & Entity::data() const
{
    Assert(m_world, "Entity is hollow");

    return m_world->entityData(m_id);
}

bool Entity::hasComponent(TypeID::value_t c) const
{
    Assert(m_world, "Entity is hollow");

    return m_world->component(m_id, c) != nullptr;
}

ComponentBase & Entity::component(TypeID::value_t c)
{
    Assert(m_world, "Entity is hollow");

    auto component = m_world->component(m_id, c);
    Assert(!!component, "No such component");

    return *component;
}

const ComponentBase & Entity::component(TypeID::value_t c) const
{
    Assert(m_world, "Entity is hollow");

    auto component = m_world->component(m_id, c);
    Assert(!!component, "No such component");

    return *component;
}

void Entity::addComponent(TypeID::value_t c, std::unique_ptr<ComponentBase> component)
{
    Assert(m_world, "Entity is hollow");

    m_world->addComponent(m_id, c, std::move(component));
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

