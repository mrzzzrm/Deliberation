#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/Entity.h>

namespace deliberation
{
class ComponentPrototypeBase;

class EntityPrototype
{
public:
    EntityPrototype(const std::string & key);
    virtual ~EntityPrototype() = default;

    const std::string & key() const { return m_key; }

    u32  order() const { return m_order; }
    void setOrder(u32 order) { m_order = order; }

    const Json & json() const { return m_json; }
    void         setJson(const Json & json) { m_json = json; }

    const std::vector<std::shared_ptr<ComponentPrototypeBase>> &
    componentPrototypes()
    {
        return m_componentPrototypes;
    }
    const std::vector<std::shared_ptr<EntityPrototype>> &
    baseEntityPrototypes() const
    {
        return m_baseEntityPrototypes;
    }

    void addComponentPrototype(
        const std::shared_ptr<ComponentPrototypeBase> & componentPrototype);
    void addBaseEntityPrototype(
        const std::shared_ptr<EntityPrototype> & entityPrototype);

    void applyToEntity(Entity & entity);

    bool isEntityDirty(const Entity & entity) const;

    void updateEntities();

private:
    std::vector<std::shared_ptr<ComponentPrototypeBase>> m_componentPrototypes;
    std::vector<std::shared_ptr<EntityPrototype>>        m_baseEntityPrototypes;
    std::vector<Entity>                                  m_entities;
    std::string                                          m_key;
    u32                                                  m_order = 0;
    Json                                                 m_json;
};
}