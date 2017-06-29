#include <Deliberation/ECS/Level.h>

#include <fstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{

Level::Level(const std::shared_ptr<PrototypeManager> & prototypeManager, const std::string & path):
    m_world(prototypeManager->world()),
    m_prototypeManager(prototypeManager),
    m_path(path)
{
    reload();
}

std::string Level::convertToUid(const std::string & lid)
{
    return m_path + "/" + lid;
}

void Level::reload()
{
    std::ifstream stream(m_path);
    Assert(stream.is_open(), "Level: Failed to open '" + m_path + "'");

    Json levelJson;
    stream >> levelJson;

    if (levelJson.empty()) return;

    try
    {
        for (auto &levelEntityJson : levelJson) {
            auto &lid = levelEntityJson["LID"];
            auto iter = m_levelEntityByUid.find(lid);

            if (iter == m_levelEntityByUid.end()) {
                auto uid = convertToUid(lid);

                auto &levelEntity = m_levelEntityByUid.emplace(lid.get<std::string>(),
                                                               std::make_shared<LevelEntity>()).first->second;
                levelEntity->entityPrototype = m_prototypeManager->getOrCreateEntityPrototype(uid);
                levelEntity->entityPrototype->setJson(levelEntityJson);
                levelEntity->lid = lid;
                levelEntity->entity = m_prototypeManager->createEntity(uid, levelEntityJson.value("Name", "Unnamed " +
                                                                                                          levelEntityJson["LID"].get<std::string>()));
            } else {
                iter->second->entityPrototype->setJson(levelEntityJson);
            }
        }
    }
    catch (const std::exception & e)
    {
        std::cout << "Error parsing JSON: \n" << levelJson << "\nError:" << e.what() << std::endl;
        Fail("");
    }


    m_prototypeManager->updateEntities();
}

}