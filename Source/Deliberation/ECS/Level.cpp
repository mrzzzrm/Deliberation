#include <Deliberation/ECS/Level.h>

#include <fstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{
Level::Level(
    const std::shared_ptr<EntityPrototypeManager> & prototypeManager,
    const std::string &                       path)
    : m_world(prototypeManager->world())
    , m_prototypeManager(prototypeManager)
    , m_path(path)
{
    reload();
}

std::shared_ptr<LevelEntity> Level::getEntityByLid(const std::string & lid)
{
    const auto iter = m_levelEntityByLid.find(lid);

    if (iter == m_levelEntityByLid.end())
        return {};
    else
        return iter->second;
}

std::string Level::convertToUid(const std::string & lid)
{
    return m_path + "/" + lid;
}

void Level::reload()
{
    std::ifstream stream(m_path);
    AssertM(stream.is_open(), "Level: Failed to open '" + m_path + "'");

    Json levelJson;
    stream >> levelJson;

    if (levelJson.empty()) return;

    try {
        // Children
        {
            const auto iter = levelJson.find("Children");
            if (iter != levelJson.end())
            {
                for (const auto & child : *iter)
                {
                    const auto path = GameDataPath("Data/Levels/" + child.get<std::string>() + ".json");

                    auto iter2 = m_childByPath.find(path);

                    if (iter2 == m_childByPath.end())
                    {
                        iter2 = m_childByPath.emplace(path, std::make_shared<Level>(m_prototypeManager, path)).first;
                    }

                    iter2->second->reload();
                }
            }
        }

        // Entities
        {
            const auto iter = levelJson.find("Entities");

            if (iter != levelJson.end()) {
                for (auto &levelEntityJson : *iter) {
                    auto &lid = levelEntityJson["LID"];
                    auto iter = m_levelEntityByLid.find(lid);

                    if (iter == m_levelEntityByLid.end()) {
                        auto uid = convertToUid(lid);

                        auto &levelEntity = m_levelEntityByLid
                            .emplace(
                                lid.get<std::string>(),
                                std::make_shared<LevelEntity>())
                            .first->second;
                        levelEntity->entityPrototype =
                            m_prototypeManager->getOrCreateEntityPrototype(uid);
                        levelEntity->entityPrototype->setJson(levelEntityJson);
                        levelEntity->lid = lid;
                        levelEntity->entity = m_prototypeManager->createEntity(
                            uid,
                            levelEntityJson.value(
                                "Name",
                                "Unnamed " +
                                levelEntityJson["LID"].get<std::string>()));
                    } else {
                        iter->second->entityPrototype->setJson(levelEntityJson);
                    }
                }
            }
        }
    }
    catch (const std::exception & e)
    {
        std::cout << "Error parsing JSON: \n"
                  << levelJson << "\nError:" << e.what() << std::endl;
        Fail("");
    }

    m_prototypeManager->updateEntities();
}
}