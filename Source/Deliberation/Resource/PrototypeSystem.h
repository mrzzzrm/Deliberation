#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Resource/PrototypeManager.h>

namespace deliberation {

class PrototypeSystem :
    public System<PrototypeSystem>
{
public:
    PrototypeSystem(World & world):
        Base(world),
        m_manager(std::make_shared<PrototypeManager>(world))
    {}

    std::shared_ptr<const PrototypeManager> manager() const { return m_manager; }
    const std::shared_ptr<PrototypeManager> & manager() { return m_manager; }

protected:
    std::shared_ptr<PrototypeManager> m_manager;
};

}