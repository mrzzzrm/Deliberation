#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>

namespace deliberation
{

class EntityPrototypeSystem:
    public System<EntityPrototypeSystem>
{
public:
    EntityPrototypeSystem(World & world, const std::string & listPath):
        Base(world),
        m_manager(std::make_shared<EntityPrototypeManager>(world, listPath))
    {
        m_manager->reloadList();
    }

    const std::shared_ptr<EntityPrototypeManager> & manager() { return m_manager; }
    std::shared_ptr<const EntityPrototypeManager> manager() const { return m_manager; }

protected:
    std::shared_ptr<EntityPrototypeManager> m_manager;
};


}