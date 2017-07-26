#include <Deliberation/ECS/LevelSystem.h>

#include <Deliberation/ECS/EntityPrototypeSystem.h>

namespace deliberation
{

LevelSystem::LevelSystem(World & world, const std::string & levelPath):
    Base(world),
    m_levelPath(levelPath)
{
}

void LevelSystem::onCreated()
{
    if (!m_levelPath.empty())
    {
        auto & entityPrototypeSystem = world().systemRef<EntityPrototypeSystem>();
        m_level = std::make_shared<Level>(entityPrototypeSystem.manager(), m_levelPath);
        m_level->reload();
    }
}

}