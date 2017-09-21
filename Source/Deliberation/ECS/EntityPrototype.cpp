#include <Deliberation/ECS/EntityPrototype.h>

#include <Deliberation/Core/Log.h>

#include <Deliberation/ECS/ComponentPrototypeBase.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{
EntityPrototype::EntityPrototype(const std::string & key)
    : m_key(key)
{
}

void EntityPrototype::addComponentPrototype(
    const std::shared_ptr<ComponentPrototypeBase> & componentPrototype)
{
    DELIBERATION_LOG_INNER_SCOPE("EntityPrototype");

    Log->info("EntityPrototype '{}': Adding ComponentPrototype '{}'", m_key, componentPrototype->name());
    m_componentPrototypes.emplace_back(componentPrototype);
}

void EntityPrototype::addBaseEntityPrototype(
    const std::shared_ptr<EntityPrototype> & entityPrototype)
{
    m_baseEntityPrototypes.emplace_back(entityPrototype);
}

bool EntityPrototype::isEntityDirty(const Entity & entity) const
{
    for (auto & componentPrototype : m_componentPrototypes)
    {
        if (!componentPrototype->hasComponent(entity) ||
            componentPrototype->jsonChangedFlag())
            return true;
    }
    return false;
}

void EntityPrototype::applyToEntity(Entity & entity)
{
    DELIBERATION_LOG_INNER_SCOPE("EntityPrototype");

    Log->info("'{}': Applying {} components", m_key, m_componentPrototypes.size());

    for (auto & componentPrototype : m_componentPrototypes)
    {
        componentPrototype->updateEntity(entity);
    }

    m_entities.emplace_back(entity);
}

void EntityPrototype::updateEntities()
{
    DELIBERATION_LOG_INNER_SCOPE("EntityPrototype");

    /**
     * Update entities
     */
    for (auto & entity : m_entities)
    {
        // Overhead, but keeps logs clean
        if (!entity.isValid()) continue;
        if (!isEntityDirty(entity)) continue;

        Log->info("'{}': Updating Entity '{}'", m_key, entity.name());

        for (auto & componentPrototype : m_componentPrototypes)
        {
            componentPrototype->updateEntity(entity);
        }
    }

    /**
     * Clear changed flags
     */
    for (auto & componentPrototype : m_componentPrototypes)
    {
        componentPrototype->setJson(componentPrototype->newJson());
        componentPrototype->setJsonChangedFlag(false);
    }

    /**
     * Remove invalid Entities
     */
    size_t eOut = 0;
    for (size_t eIn = 0; eIn < m_entities.size(); eIn++)
    {
        if (m_entities[eIn].isValid())
        {
            m_entities[eOut] = m_entities[eIn];
            eOut++;
        }
    };
    m_entities.resize(eOut);
};
}