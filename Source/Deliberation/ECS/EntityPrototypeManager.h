#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include <Deliberation/Core/FileModificationPoll.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/EntityPrototype.h>

namespace deliberation
{
class ComponentPrototypeBase;
class World;

class EntityPrototypeManager
{
public:
    EntityPrototypeManager(const std::shared_ptr<World> & world, const std::string & listPath);
    virtual ~EntityPrototypeManager() = default;

    const std::shared_ptr<World> & world() { return m_world;}

    const std::unordered_map<std::string, std::shared_ptr<EntityPrototype>> &
    entityPrototypeByKey() const
    {
        return m_entityPrototypeByKey;
    };

    template<typename T, typename... Args>
    void registerComponentPrototype(const std::string & name, Args &&... args);

    void reloadList();

    const std::shared_ptr<EntityPrototype> &
                                            getOrCreateEntityPrototype(const std::string & key);
    std::shared_ptr<ComponentPrototypeBase> getOrAddComponentPrototype(
        const std::shared_ptr<EntityPrototype> & entityPrototype,
        const std::string &                      componentPrototypeName);

    Entity createEntity(
        const std::string & prototypeKey, const std::string & entityName = {});
    Entity createEntity(
        const std::vector<std::string> & prototypeKeys,
        const std::string &              entityName = {});

    void updateEntities();

private:
    Json mergeJson(const Json & a, const Json & b);

protected:
    std::shared_ptr<World> m_world;

private:
    std::unordered_map<std::string, std::shared_ptr<EntityPrototype>>
                                                  m_entityPrototypeByKey;
    std::vector<std::shared_ptr<EntityPrototype>> m_entityPrototypesInOrder;
    std::unordered_map<
        std::string,
        std::function<std::shared_ptr<ComponentPrototypeBase>()>>
        m_componentPrototypeFactoryByName;

    FileModificationPoll m_listPoll;
    std::unordered_map<std::string, FileModificationPoll>
        m_entityPrototypeFilePollsByPath;
};
}

#include <Deliberation/ECS/EntityPrototypeManager.inl>