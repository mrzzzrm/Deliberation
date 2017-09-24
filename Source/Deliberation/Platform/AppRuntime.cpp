#include <Deliberation/Platform/AppRuntime.h>

#include <Deliberation/ECS/World.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/Resource/PrototypeManager.h>
#include <Deliberation/Resource/ResourceManager.h>

namespace deliberation
{

AppRuntime::AppRuntime(const std::string & name,
                                       const std::string & prefix,
                                       const std::string & entityPrototypeListPath):
    m_name(name),
    m_prefix(prefix)
{
    m_world = std::make_shared<World>();
    m_entityPrototypeManager = std::make_shared<EntityPrototypeManager>(m_world,
                                                                        entityPrototypeListPath);
    m_events = std::make_shared<EventDomain>();
    m_prototypeManager = std::make_shared<PrototypeManager>();
    m_resourceManager = std::make_shared<ResourceManager>();
}

}