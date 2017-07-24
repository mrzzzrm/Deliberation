#pragma once

#include <memory>
#include <string>

#include <Deliberation/ECS/LevelEntity.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>

namespace deliberation
{
class World;

class Level
{
public:
    Level(
        const std::shared_ptr<EntityPrototypeManager> & prototypeManager,
        const std::string &                       path);
    virtual ~Level() = default;

    std::shared_ptr<LevelEntity> getEntityByLid(const std::string & lid);

    std::string convertToUid(const std::string & lid);

    void reload();

protected:
    World &                           m_world;
    std::shared_ptr<EntityPrototypeManager> m_prototypeManager;
    std::string                       m_path;
    std::unordered_map<std::string, std::shared_ptr<LevelEntity>>
        m_levelEntityByLid;
};
}