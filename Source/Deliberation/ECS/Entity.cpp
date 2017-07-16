#include <Deliberation/ECS/Entity.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/EntityData.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{
Entity::Entity() : m_world(nullptr), m_id(ECS_INVALID_ENTITY_ID) {}

World & Entity::world() const
{
    Assert(m_world, "");
    return *m_world;
}

EntityId Entity::id() const { return m_id; }

const std::string & Entity::name() const { return data().name; }

EntityId Entity::parent() const { return data().parent; }

const std::vector<EntityId> & Entity::children() const
{
    return data().children;
}

const ComponentBitset & Entity::componentBits() const
{
    return data().componentBits;
}

bool Entity::isValid() const { return m_world && m_world->isValid(m_id); }

void Entity::scheduleRemoval()
{
    Assert(m_world, "Entity is hollow");

    m_world->scheduleEntityRemoval(m_id);
}

Entity::Entity(World & world, EntityId id) : m_world(&world), m_id(id) {}

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

ComponentBase & Entity::component(TypeID::value_t c) const
{
    Assert(m_world, "Entity is hollow");

    auto component = m_world->component(m_id, c);
    Assert(!!component, "No such component in '" + name() + "'");

    return *component;
}

void Entity::addComponent(
    TypeID::value_t c, std::shared_ptr<ComponentBase> component)
{
    Assert(m_world, "Entity is hollow");

    m_world->addComponent(m_id, c, std::move(component));
}

void Entity::scheduleComponentRemoval(TypeID::value_t c)
{
    Assert(m_world, "Entity is hollow");

    m_world->scheduleComponentRemoval(m_id, c);
}

Entity Entity::createChild(const std::string & name)
{
    Assert(m_world, "Entity is hollow");

    return m_world->createEntity(name, m_id);
}

std::string Entity::toString() const
{
    std::stringstream stream;
    stream << "{" << name() << ": ";

    const auto & bits = componentBits();

    for (size_t b = 0; b < bits.size(); b++)
    {
        if (!bits.test(b)) continue;

        const auto & component = this->component(b);
        stream << component.name() << ", ";
    }

    stream << "}";

    return stream.str();
}

bool Entity::operator<(const Entity & rhs) const
{
    if (m_world == nullptr || rhs.m_world == nullptr) return false;

    Assert(
        m_world == rhs.m_world, "Don't compare Entities from different Worlds");
    return m_id < rhs.m_id;
}

bool Entity::operator==(const Entity & rhs) const
{
    if (m_world == nullptr || rhs.m_world == nullptr) return false;

    Assert(
        m_world == rhs.m_world, "Don't compare Entities from different Worlds");
    return m_id == rhs.m_id;
}

bool Entity::operator!=(const Entity & rhs) const { return !operator==(rhs); }
}
