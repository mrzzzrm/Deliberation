#pragma once

#include <memory>
#include <string>

#include <Deliberation/ECS/LevelEntity.h>
#include <Deliberation/ECS/PrototypeManager.h>

namespace deliberation
{

class World;

class Level
{
public:
    Level(const std::shared_ptr<PrototypeManager> & prototypeManager, const std::string & path);
    virtual ~Level() = default;

    std::string convertToUid(const std::string & lid);

    void reload();

protected:
    World &                             m_world;
    std::shared_ptr<PrototypeManager>   m_prototypeManager;
    std::string                         m_path;
    std::unordered_map<std::string, std::shared_ptr<LevelEntity>>
                                        m_levelEntityByUid;

};

}