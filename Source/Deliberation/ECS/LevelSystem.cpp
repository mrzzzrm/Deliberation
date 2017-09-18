#include <Deliberation/ECS/LevelSystem.h>

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
        m_level = std::make_shared<Level>(m_levelPath);
        m_level->reload();
    }
}

}