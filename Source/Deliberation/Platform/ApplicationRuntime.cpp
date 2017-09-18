#include <Deliberation/Platform/ApplicationRuntime.h>

#include <Deliberation/ECS/World.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>

namespace deliberation
{

ApplicationRuntime::ApplicationRuntime(const std::string & name, const std::string & prefix):
    m_name(name),
    m_prefix(prefix)
{
    m_world = std::make_shared<World>();
    m_entityPrototypeManager = std::make_shared<EntityPrototypeManager>(*m_world,
                                                                        GameDataPath("Data/EntityPrototypes/list.json"));
}

}