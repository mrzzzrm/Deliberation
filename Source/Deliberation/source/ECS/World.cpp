#include <Deliberation/ECS/World.h>

#include <iostream>
#include <sstream>

#include <Deliberation/Core/Assert.h>

#define VERBOSE 0

namespace deliberation
{

World::World():
    m_entityIDCounter(1)
{

}

World::~World()
{
#if VERBOSE
    std::cout << "World::~World()" << std::endl;
#endif

    for (auto & entity : m_entities)
    {
        if (isValid(entity.id))
        {
            remove(entity.id);
        }
    }
}

EventManager & World::eventManager()
{
    return m_eventManager;
}

EntityData & World::entityData(entity_id_t id)
{
    Assert(isValid(id), "");

    return m_entities[entityIndex(id)];
}

Entity World::createEntity(const std::string & name, entity_id_t parent)
{
    auto id = m_entityIDCounter++;
    auto index = m_entities.emplace(id, name, parent);
    m_entityIndexByID[id] = index;

#if VERBOSE
    std::cout << "World::createEntity(): Allocated index=" << index << " for Entity '" << name << "'; parent=" << (int)parent << std::endl;
#endif

    auto & entity = entityData(id);

    if (isValid(parent))
    {
        m_entities[entityIndex(parent)].children.push_back(id);
    }

    entity.componentSetup = componentSetup(entity.componentBits);

    return Entity(*this, id);
}

void World::update(float seconds)
{
    for (auto & pair : m_systems)
    {
        pair.second->beforeUpdate();
    }

    for (auto & pair : m_systems)
    {
        pair.second->update(seconds);
    }
}

std::string World::toString() const
{
    std::stringstream stream;

    for (auto & entity : m_entities)
    {
        if (!isValid(entity.id))
        {
            continue;
        }

        stream << "Entity " << entity.id << "/" << entity.name;
        if (isValid(entity.parent))
        {
            stream << "; parent = " << entity.parent << "/" << m_entities[entity.parent].name;
        }
        stream << std::endl;

        for (auto & pair : m_components)
        {
            auto & components = pair.second;

            if (components.contains(entity.id))
            {
                stream << "  Component " << components.at(entity.id)->name() << std::endl;
            }
        }
    }

    return stream.str();
}

bool World::isValid(entity_id_t id) const
{
    return m_entityIndexByID.find(id) != m_entityIndexByID.end();
}

void World::remove(entity_id_t id)
{
    Assert(isValid(id), "");

    auto i = entityIndex(id);
    auto & entity = m_entities[i];
    auto * componentSetup = entity.componentSetup;

#if VERBOSE
    std::cout << "World::remove(): id=" << id << "; name=" << entity.name << std::endl;
    std::cout << "  num children=" << entity.children.size() << std::endl;
#endif

    for (auto child : entity.children)
    {
        remove(child);
    }


    for (auto componentIndex : componentSetup->componentIndices)
    {
        removeComponent(id, componentIndex);
    }


    m_entities.erase(i);
    m_entityIndexByID.erase(id);
}

std::shared_ptr<ComponentBase> World::component(entity_id_t id, TypeID::value_t index)
{
    Assert(isValid(id), "");

    auto i = entityIndex(id);

    if (!m_components.contains(index))
    {
        return nullptr;
    }

    if (!m_components.at(index).contains(i))
    {
        return nullptr;
    }

    return m_components.at(index).at(i);
}

std::shared_ptr<const ComponentBase> World::component(entity_id_t id, TypeID::value_t index) const
{
    Assert(isValid(id), "");

    auto i = entityIndex(id);

    if (!m_components.contains(index))
    {
        return nullptr;
    }

    if (!m_components.at(index).contains(i))
    {
        return nullptr;
    }

    return m_components.at(index).at(i);
}

void World::addComponent(entity_id_t id, TypeID::value_t index, std::unique_ptr<ComponentBase> component)
{
    Assert(isValid(id), "");


    auto i = entityIndex(id);
    auto & entity = m_entities[i];
    auto * prevComponentSetup = entity.componentSetup;

    entity.componentBits.set(index);
    entity.componentSetup = componentSetup(entity.componentBits);
    m_components[index][i] = std::move(component);

#if VERBOSE
    std::cout << "World::addComponent()" << std::endl;
#endif

    for (auto systemIndex : entity.componentSetup->systemIndices)
    {
#if VERBOSE
    std::cout << "  Checking system " << systemIndex << " (" << prevComponentSetup->systemBits.test(systemIndex) << ")" << std::endl;
#endif
        if (!prevComponentSetup->systemBits.test(systemIndex))
        {
#if VERBOSE
    std::cout << "  Adding entity to system" << std::endl;
#endif
            auto & system = *m_systems[systemIndex];
            Entity entity(*this, id);
            system.addEntity(entity);
        }
    }
}

void World::removeComponent(entity_id_t id, TypeID::value_t index)
{
    auto i = entityIndex(id);
    auto & entity = m_entities[i];
    auto * prevComponentSetup = entity.componentSetup;

    entity.componentBits.reset(index);
    entity.componentSetup = componentSetup(entity.componentBits);
    m_components[index].erase(i);

    for (auto systemIndex : prevComponentSetup->systemIndices)
    {
        if (!entity.componentSetup->systemBits.test(systemIndex))
        {
            auto & system = *m_systems[systemIndex];
            Entity entity(*this, id);
            system.removeEntity(entity);
        }
    }
}

std::size_t World::entityIndex(entity_id_t id) const
{
    auto i = m_entityIndexByID.find(id);
    Assert(i != m_entityIndexByID.end(), "");

    return i->second;
}

EntityComponentSetup * World::componentSetup(const ComponentBitset & componentBits)
{
    auto i = m_entityComponentSetups.find(componentBits);
    if (i != m_entityComponentSetups.end())
    {
        return &i->second;
    }

#if VERBOSE
    std::cout << "World: New component setup for bits " << componentBits.to_string() << std::endl;
#endif

    auto & setup = m_entityComponentSetups[componentBits];

    for (auto b = 0; b < ECS_MAX_NUM_COMPONENTS; b++)
    {
        if (componentBits.test(b))
        {
#if VERBOSE
            std::cout << "  Contains component " << b << std::endl;
#endif
            setup.componentIndices.push_back(b);
        }
    }
    for (std::size_t s = 0; s < m_systems.keyUpperBound(); s++)
    {
        if (!m_systems.contains(s))
        {
            continue;
        }

        auto & system = *m_systems[s];
        if (system.filter().accepts(componentBits))
        {
#if VERBOSE
            std::cout << "  In system " << s << std::endl;
#endif
            setup.systemIndices.push_back(s);
            setup.systemBits.set(s);
        }
    }

    return &setup;
}

}

