#pragma once

#include <memory>
#include <string>

#include <Deliberation/ECS/LevelEntity.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/ECS/AbstractActivity.h>

namespace deliberation
{
class World;

class Level
{
public:
    Level(
        const std::string &                       path);
    virtual ~Level() = default;

    std::shared_ptr<LevelEntity> getEntityByLid(const std::string & lid);

    std::string convertToUid(const std::string & lid);

    void reload();

protected:
    std::string                       m_path;
    std::unordered_map<std::string, std::shared_ptr<LevelEntity>>
        m_levelEntityByLid;
    std::unordered_map<std::string, std::shared_ptr<Level>> m_childByPath;
    std::unordered_map<std::string, std::shared_ptr<AbstractActivity>> m_activityByLid;
};
}