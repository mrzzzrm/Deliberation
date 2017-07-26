#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/Level.h>

namespace deliberation
{

class LevelSystem:
    public System<LevelSystem>
{
public:
    LevelSystem(World & world, const std::string & levelPath = {});

    const std::shared_ptr<Level> & level() { return m_level; }
    std::shared_ptr<const Level> level() const { return m_level; }

    void setLevel(const std::shared_ptr<Level> & level) { m_level = level; }

    void onCreated() override;

private:
    std::shared_ptr<Level> m_level;
    std::string m_levelPath;
};

}
