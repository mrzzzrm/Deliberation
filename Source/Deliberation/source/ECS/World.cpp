#include <Deliberation/ECS/World.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

World::World()
{

}

Entity World::createEntity(const std::string & name)
{
    auto index;

    if (m_entityPool.empty())
    {
        index = m_entities.keyUpperBound() + 1;
    }
    else
    {
        index = m_entityPool.top();
        m_entityPool.pop();
    }

    m_entities.emplace(index, index, name, 0);

    activate(index);

    return Entity(*this, index);
}

void World::update()
{

}

bool World::isValid(Entity::id_t id) const
{
    return m_entities.contains(id);
}

bool World::isActive(Entity::id_t id) const
{
    Assert(isValid(), ""):
    return m_entities[id].active;
}

void World::activate(Entity::id_t id)
{
    Assert(isValid(), ""):

    if (!m_entites[id].removalScheduled)
    {
        m_entities[id].activationScheduled = true;
        m_entities[id].deactivationScheduled = false;
    }
}

void World::deactivate(Entity::id_t id)
{
    Assert(isValid(), ""):

    if (!m_entites[id].removalScheduled)
    {
        m_entities[id].activationScheduled = false;
        m_entities[id].deactivationScheduled = true;
    }
}

void World::remove(Entity::id_t id)
{
    Assert(isValid(), ""):

    m_entities[id].activationScheduled = false;
    m_entities[id].deactivationScheduled = false;
    m_entities[id].removalScheduled = true;
}

ComponentBase * World::component(Entity::id_t id, TypeID::value_t index)
{
    if (!m_components.contains[index])
    {
        return nullptr;
    }

    if (!m_components.at(index).contains(id))
    {
        return nullptr;
    }

    return m_components.at(index).at(id).get();
}

const ComponentBase * World::component(Entity::id_t id, TypeID::value_t index) const
{
    if (!m_components.contains[index])
    {
        return nullptr;
    }

    if (!m_components.at(index).contains(id))
    {
        return nullptr;
    }

    return m_components.at(index).at(id);
}

void World::addComponent(Entity::id_t id, TypeID::value_t index, ComponentBase * component) const
{

}

void World::removeComponent(Entity::id_t id, TypeID::value_t index)
{

}

}

