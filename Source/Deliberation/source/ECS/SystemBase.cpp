#include <Deliberation/ECS/System.h>

namespace deliberation
{

SystemBase::SystemBase(World & world, const ComponentFilter & filter):
    m_world(world),
    m_filter(filter)
{

}

SystemBase::~SystemBase()
{

}

World & SystemBase::world()
{
    return m_world;
}

bool SystemBase::accepts(const Entity & entity)
{
    return m_filter.accepts(entity.componentBits());
}

void SystemBase::addEntity(Entity & entity)
{
    Assert(&entity.world() == &m_world, "");

    m_entities.insert(entity.id());
    onEntityAdded(entity);
}

void SystemBase::removeEntity(Entity & entity)
{
    Assert(&entity.world() == &m_world, "");

    m_entities.erase(entity.id());
    onEntityRemoved(entity);
}

void SystemBase::update(float seconds)
{
    for (auto id : m_entities)
    {
        Entity entity(m_world, id);
        onUpdate(entity, seconds);
    }
}

void SystemBase::onEntityAdded(Entity & entity)
{
    // dummy
}

void SystemBase::onEntityRemoved(Entity & entity)
{
    // dummy
}

void SystemBase::onUpdate(Entity & entity, float seconds)
{
    // dummy
}


}
