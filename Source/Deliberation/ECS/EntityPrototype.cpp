#include <Deliberation/ECS/EntityPrototype.h>

#include <Deliberation/ECS/ComponentPrototypeBase.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{

EntityPrototype::EntityPrototype(World & world, const std::string & key):
    m_world(world),
    m_key(key)
{}

void EntityPrototype::addComponentPrototype(const std::shared_ptr<ComponentPrototypeBase> & componentPrototype)
{
    m_componentPrototypes.emplace_back(componentPrototype);
}

void EntityPrototype::addBaseEntityPrototype(const std::shared_ptr<EntityPrototype> & entityPrototype)
{
    m_baseEntityPrototypes.emplace_back(entityPrototype);
}

bool EntityPrototype::isEntityDirty(const Entity & entity) const
{
    for (auto & componentPrototype : m_componentPrototypes)
    {
        if (!componentPrototype->hasComponent(entity) || componentPrototype->jsonChangedFag()) return true;
    }
    return false;
}

Entity EntityPrototype::createEntity(const std::string & name)
{
    auto entity = m_world.createEntity(name);

    std::cout << "EntityPrototype: Creating " << m_componentPrototypes.size() << " components" << std::endl;

    for (auto & componentPrototype : m_componentPrototypes)
    {
        componentPrototype->updateEntity(entity);
    }

    m_entities.emplace_back(entity);

    return entity;
}

void EntityPrototype::updateEntities()
{
    /**
     * Update entities
     */
    for (auto & entity : m_entities)
    {
        if (!entity.isValid()) continue;
        if (!isEntityDirty(entity)) continue; // Overhead, but keeps logs clean

        std::cout << "Updating Entity '" << entity.name() << "'" << std::endl;

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