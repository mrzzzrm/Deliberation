#pragma once

#include <functional>
#include <memory>
#include <string>

#include <Deliberation/Core/FileModificationPoll.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/EntityPrototype.h>

namespace deliberation
{

class ComponentPrototypeBase;
class World;

class PrototypeManager
{
public:
    PrototypeManager(World & world, const std::string & listPath);
    virtual ~PrototypeManager() = default;

    World & world() const { return m_world; }

    template<typename T>
    void registerComponentPrototype(const std::string & name);

    void reloadList();

    const std::shared_ptr<EntityPrototype> & getOrCreateEntityPrototype(const std::string & key);
    std::shared_ptr<ComponentPrototypeBase> getOrAddComponentPrototype(
        const std::shared_ptr<EntityPrototype> & entityPrototype, const std::string & componentPrototypeName);

    Entity createEntity(const std::string & prototypeKey, const std::string & entityName = {});

    void updateEntities();

private:
    Json mergeJson(const Json & a, const Json & b);

private:
    World &                                 m_world;
    std::unordered_map<std::string,
        std::shared_ptr<EntityPrototype>>   m_entityPrototypeByKey;
    std::vector<std::shared_ptr<EntityPrototype>>
                                            m_entityPrototypesInOrder;
    std::unordered_map<std::string,
        std::function<std::shared_ptr<ComponentPrototypeBase>()>>
                                            m_componentPrototypeFactoryByName;

    FileModificationPoll                    m_listPoll;
    std::unordered_map<std::string, FileModificationPoll>
                                            m_entityPrototypeFilePollsByPath;
};

}

#include <Deliberation/ECS/PrototypeManager.inl>