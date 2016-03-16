#include <Deliberation/ECS/World.h>

#include <iostream>
#include <sstream>

#include <Deliberation/Core/Assert.h>

#define VERBOSE 1

using namespace deliberation::detail;

namespace deliberation
{

World::World()
{

}

World::~World()
{
#if VERBOSE
    std::cout << "World::~World()" << std::endl;
#endif

    for (auto & entity : m_entities)
    {
        if (isValid(entity.second.id))
        {
            remove(entity.second.id);
        }
    }
    update();
}

EntityData & World::entityData(Entity::id_t id)
{
    Assert(isValid(id), "");
    return m_entities[id];
}

Entity World::createEntity(const std::string & name, Entity::id_t parent)
{
    std::size_t index;

    if (m_entityPool.empty())
    {
        index = m_entities.keyUpperBound() + 1;
    }
    else
    {
        index = m_entityPool.top();
        m_entityPool.pop();
    }

#if VERBOSE
    std::cout << "World::createEntity(): Allocated id=" << index << " for Entity '" << name << "'; parent=" << (int)parent << std::endl;
#endif

    m_entities.emplace(index, index, name, parent);

    if (isValid(parent))
    {
        m_entities[parent].children.push_back(index);
    }

    activate(index);

    return Entity(*this, index);
}

void World::update()
{
    /*
        Determine actual Entity transition by overriding childrens transition
    */
    for (auto t = 0; t < m_scheduledTransitions.size(); t++)
    {
        auto & transitions = m_scheduledTransitions[t];

#if VERBOSE
        if (transitions.size() > 0)
        {
            std::cout << "World::update(): Scheduled for transition " << t << std::endl;
        }
#endif

        for (auto id : transitions)
        {
#if VERBOSE
            std::cout << "  " << id << "/" << m_entities[id].name << std::endl;
#endif
            propagateTransition(id, (EntityTransition)t);
        }
    }

    /*
        Invalidate transitions that got overriden
        in the previous step
    */
    for (auto t = 0; t < m_propagatedTransitions.size(); t++)
    {
        auto & transitions = m_propagatedTransitions[t];

        for (auto & id : transitions)
        {
            Assert(isValid(id), "");

            auto & entity = m_entities[id];
            if (entity.transition != t)
            {
                id = Entity::INVALID_ID;
            }
        }
    }

    /*
        Establish transition order
    */
    for (auto t = 0; t < m_propagatedTransitions.size(); t++)
    {
        auto & transitions = m_propagatedTransitions[t];

#if VERBOSE
        if (transitions.size() > 0)
        {
            std::cout << "World::update(): Propagated to transition " << t << std::endl;
        }
#endif
        for (auto & id : transitions)
        {
            if (!isValid(id))
            {
                continue;
            }
#if VERBOSE
            std::cout << "  " << id << "/" << m_entities[id].name << std::endl;
#endif

            insertChildrenTransitions(id, (EntityTransition)t);
        }
    }

    /*
        Transition
    */
    for (auto t = 0; t < m_orderedTransitions.size(); t++)
    {
        auto & transitions = m_orderedTransitions[t];

#if VERBOSE
        if (transitions.size() > 0)
        {
            std::cout << "World::update(): Performing transition " << t << std::endl;
        }
#endif
        for (auto & id : transitions)
        {
            if (!isValid(id))
            {
                continue;
            }

            auto & entity = m_entities[id];

            if (entity.state == t)
            {
                continue;
            }

            switch (t)
            {
            case EntityTransition_Activate:
                performActivation(entity);
                break;
            case EntityTransition_Deactivate:
                performDeactivation(entity);
                break;
            case EntityTransition_Remove:
                performRemoval(entity);
                break;
            default:
                Fail("");
            }

            entity.transition = EntityTransition_None;
            entity.scheduledTransitions.reset();
            entity.propagatedTransitions.reset();
            entity.orderedTransitions.reset();
        }
    }

    /*
        Clear Transitions
    */
    for (auto t = 0; t < m_scheduledTransitions.size(); t++)
    {
        m_scheduledTransitions[t].clear();
        m_propagatedTransitions[t].clear();
        m_orderedTransitions[t].clear();
    }
}

bool World::isValid(Entity::id_t id) const
{
    return m_entities.contains(id);
}

bool World::isActive(Entity::id_t id) const
{
    Assert(isValid(id), "");
    return m_entities[id].state == EntityState_Activated;
}

void World::activate(Entity::id_t id)
{
    scheduleTransition(id, detail::EntityTransition_Activate);
}

void World::deactivate(Entity::id_t id)
{
    scheduleTransition(id, detail::EntityTransition_Deactivate);
}

void World::remove(Entity::id_t id)
{
    scheduleTransition(id, detail::EntityTransition_Remove);
}

ComponentBase * World::component(Entity::id_t id, TypeID::value_t index)
{
    Assert(isValid(id), "");

    if (!m_components.contains(index))
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
    Assert(isValid(id), "");

    if (!m_components.contains(index))
    {
        return nullptr;
    }

    if (!m_components.at(index).contains(id))
    {
        return nullptr;
    }

    return m_components.at(index).at(id).get();
}

void World::addComponent(Entity::id_t id, TypeID::value_t index, ComponentBase * component)
{
    Assert(isValid(id), "");

    m_components[index][id].reset(component);
}

void World::removeComponent(Entity::id_t id, TypeID::value_t index)
{
    m_components[index].erase(id);
}

void World::scheduleTransition(Entity::id_t id, detail::EntityTransition transition)
{
    Assert(isValid(id), "");

    auto & entity = m_entities[id];

    entity.transition = transition;

    if (!entity.scheduledTransitions[transition])
    {
        m_scheduledTransitions[transition].push_back(id);
        entity.scheduledTransitions[transition] = true;
    }
}

void World::propagateTransition(Entity::id_t id, detail::EntityTransition transition)
{
    Assert(isValid(id), "");

    auto & entity = m_entities[id];

    entity.transition = transition;

    if (!entity.propagatedTransitions[transition])
    {
        m_propagatedTransitions[transition].push_back(id);
        entity.propagatedTransitions[transition] = true;
    }

    for (auto child : entity.children)
    {
        propagateTransition(child, transition);
    }
}

void World::insertChildrenTransitions(Entity::id_t id, detail::EntityTransition transition)
{
    Assert(isValid(id), "");

    auto & entity = m_entities[id];

    if (entity.orderedTransitions[transition])
    {
        return;
    }

    for (auto child : entity.children)
    {
        insertChildrenTransitions(child, transition);
    }

    m_orderedTransitions[transition].push_back(id);
    entity.orderedTransitions[transition] = true;
}

std::string World::toString() const
{
    std::stringstream stream;

    for (auto & pair : m_entities)
    {
        auto & entity = pair.second;

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

void World::performActivation(detail::EntityData & entity)
{
#if VERBOSE
    std::cout << "  Activating " << entity.id << "/" << entity.name << std::endl;
#endif

    entity.state = EntityState_Activated;
}

void World::performDeactivation(detail::EntityData & entity)
{
#if VERBOSE
    std::cout << "  Deactivating " << entity.id << "/" << entity.name << std::endl;
#endif

    entity.state = EntityState_Deactivated;
}

void World::performRemoval(detail::EntityData & entity)
{
#if VERBOSE
    std::cout << "  Removing " << entity.id << "/" << entity.name << std::endl;
#endif

    m_entityPool.push(entity.id);
    entity.id = Entity::INVALID_ID;
}

}

