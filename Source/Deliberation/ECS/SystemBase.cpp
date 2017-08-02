#include <Deliberation/ECS/System.h>

namespace deliberation
{
SystemBase::SystemBase(World & world, const ComponentFilter & filter)
    : m_world(world), m_filter(filter)
{
}

World & SystemBase::world() { return m_world; }

const ComponentFilter & SystemBase::filter() const { return m_filter; }

bool SystemBase::accepts(const Entity & entity)
{
    return m_filter.accepts(entity.componentBits());
}

void SystemBase::addEntity(Entity & entity)
{
    Assert(&entity.world() == &m_world, "");

    EntityEntry entry;
    entry.id = entity.id();
    entry.active = false;

    m_entityIndexByID[entity.id()] = m_entities.insert(std::move(entry));

    onEntityAdded(entity);
}

void SystemBase::removeEntity(Entity & entity)
{
    Assert(&entity.world() == &m_world, "");

    auto i = m_entityIndexByID.find(entity.id());
    Assert(i != m_entityIndexByID.end(), "Entity '" + entity.name() + "' not in System '" + name() + "'");

    m_entities.erase(i->second);
    m_entityIndexByID.erase(i);

    onEntityRemoved(entity);
}

void SystemBase::frameBegin() { onFrameBegin(); }

void SystemBase::beforeGameUpdate()
{
    for (auto & entry : m_entities)
    {
        entry.active = true;
    }
}

void SystemBase::gameUpdate(float seconds)
{
    onGameUpdate(seconds);

    for (auto & entry : m_entities)
    {
        if (!entry.active) continue;

        Entity entity(m_world, entry.id);
        onEntityGameUpdate(entity, seconds);
    }
}

void SystemBase::frameUpdate(float seconds) { onFrameUpdate(seconds); }

void SystemBase::prePhysicsUpdate(float seconds)
{
    onPrePhysicsUpdate(seconds);

    for (auto & entry : m_entities)
    {
        if (!entry.active) continue;

        Entity entity(m_world, entry.id);
        onEntityPrePhysicsUpdate(entity, seconds);
    }
}

void SystemBase::postPhysicsUpdate(float seconds)
{
    onPostPhysicsUpdate(seconds);

    for (auto & entry : m_entities)
    {
        if (!entry.active) continue;

        Entity entity(m_world, entry.id);
        onEntityPostPhysicsUpdate(entity, seconds);
    }
}

void SystemBase::frameComplete(float seconds) { onFrameComplete(seconds); }

void SystemBase::onEntityAdded(Entity & entity)
{
    // dummy
}

void SystemBase::onEntityRemoved(Entity & entity)
{
    // dummy
}

void SystemBase::onEntityPrePhysicsUpdate(
    Entity & entity, float physicsTimestep)
{
}

void SystemBase::onFrameBegin() {}

void SystemBase::onPrePhysicsUpdate(float seconds) {}

void SystemBase::onFrameComplete(float seconds) {}
}
