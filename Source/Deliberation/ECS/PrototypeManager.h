#pragma once

#include <functional>
#include <memory>
#include <string>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/EntityPrototype.h>

namespace deliberation
{

class ComponentPrototypeBase;
class World;

class PrototypeManager
{
public:
    PrototypeManager(World & world);
    virtual ~PrototypeManager() = default;

    template<typename T>
    void registerComponentPrototype(const std::string & name);

    void loadOrReloadList(const std::string & listPath);

    const std::shared_ptr<EntityPrototype> & getOrCreateEntityPrototype(const std::string & key);

    Entity createEntity(const std::string & prototypeKey, const std::string & entityName = {});

    void updateEntities();

private:
    void updateEntityPrototypeOrder();

private:
    World &                                 m_world;
    std::unordered_map<std::string,
        std::shared_ptr<EntityPrototype>>   m_entityPrototypeByKey;
    std::vector<std::shared_ptr<EntityPrototype>>
                                            m_entityPrototypesInOrder;
    std::unordered_map<std::string,
        std::function<std::shared_ptr<ComponentPrototypeBase>()>>
                                            m_componentPrototypeFactoryByName;
};

}

#include <Deliberation/ECS/PrototypeManager.inl>