#include <Deliberation/ECS/PrototypeManager.h>

#include <fstream>
#include <limits>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/FilesystemUtils.h>
#include <Deliberation/Core/Json.h>

#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{

PrototypeManager::PrototypeManager(World & world):
    m_world(world)
{

}

const std::shared_ptr<EntityPrototype> & PrototypeManager::getOrCreateEntityPrototype(const std::string & key)
{
    auto iter = m_entityPrototypeByKey.find(key);

    if (iter == m_entityPrototypeByKey.end())
    {
        iter = m_entityPrototypeByKey.emplace(key, std::make_shared<EntityPrototype>(m_world, key)).first;
    }

    return iter->second;
}

void PrototypeManager::loadOrReloadList(const std::string & listPath)
{
    /**
     * TODO Move to <filesystem> as soon as it becomes widely available
     */

    const auto listDir = GetDirFromPath(listPath);

    std::ifstream listFile(listPath);
    Assert(listFile.is_open(), "Failed to open prototype list '" + listPath + "'");

    Json listJson;
    listFile >> listJson;

    /**
     * Make sure all EntityPrototypes exist
     */
    for (const auto & prototypeName : listJson["Prototypes"])
    {
        getOrCreateEntityPrototype(prototypeName);
    }

    /**
     * Establish all Derivate->Base connections and set Component JSONs
     */
    for (const auto & prototypeName : listJson["Prototypes"])
    {
        auto entityPrototype = getOrCreateEntityPrototype(prototypeName);

        const auto prototypePath = listDir + prototypeName.get<std::string>() + ".json";

        /**
         * TODO(me) Skip if file didn't change timestamps
         */

        std::cout << "PrototypeManager: Loading EntityPrototype '" + prototypeName.get<std::string>() + "'" << std::endl;

        std::ifstream prototypeFile(prototypePath);
        Assert(prototypeFile.is_open(), "Failed to open prototype '" + prototypePath + "'");

        Json prototypeJson;
        prototypeFile >> prototypeJson;

        entityPrototype->setJson(prototypeJson);

        if (prototypeJson.count("Bases") > 0)
        {
            for (auto & baseEntityPrototypeName : prototypeJson["Bases"])
            {
                auto & baseEntityPrototypes = entityPrototype->baseEntityPrototypes();
                const auto iter = std::find_if(baseEntityPrototypes.begin(), baseEntityPrototypes.end(), [&]
                    (const std::shared_ptr<EntityPrototype> & exisitingBaseEntityPrototype) {
                    return baseEntityPrototypeName == exisitingBaseEntityPrototype->key();
                });

                if (iter == baseEntityPrototypes.end())
                {
                    auto baseEntityPrototype = getOrCreateEntityPrototype(baseEntityPrototypeName);
                    entityPrototype->addBaseEntityPrototype(baseEntityPrototype);
                }
            }
        }

        if (prototypeJson.count("Components") > 0)
        {
            for (auto & pair : Json::iterator_wrapper(prototypeJson["Components"]))
            {
                auto componentPrototypeName = pair.key();

                {
                    auto & componentPrototypes = entityPrototype->componentPrototypes();

                    const auto iter = std::find_if(componentPrototypes.begin(), componentPrototypes.end(),
                    [&] (const std::shared_ptr<ComponentPrototypeBase> & componentPrototype) {
                        return componentPrototypeName == componentPrototype->name();
                    });

                    if (iter != componentPrototypes.end()) continue;
                }

                auto componentPrototypeJson = pair.value();

                auto iter = m_componentPrototypeFactoryByName.find(componentPrototypeName);
                Assert(iter != m_componentPrototypeFactoryByName.end(),
                       "Component '" + componentPrototypeName + "' not registered");

                auto componentPrototype = iter->second();
                componentPrototype->setJson(componentPrototypeJson);

                entityPrototype->addComponentPrototype(componentPrototype);
            }
        }
    }

    /**
     *
     */
    updateEntityPrototypeOrder();

    /**
     * Patch ComponentPrototypes JSON
     */
    for (auto & entityPrototype : m_entityPrototypesInOrder)
    {
        for (auto & componentPrototype : entityPrototype->componentPrototypes())
        {
            auto & name = componentPrototype->name();
            auto foundBaseEntityPrototype = false;

            for (auto & baseEntityPrototype : entityPrototype->baseEntityPrototypes())
            {
                auto & prototypes = baseEntityPrototype->componentPrototypes();

                const auto iter = std::find_if(prototypes.begin(), prototypes.end(),
                                         [&](const std::shared_ptr<ComponentPrototypeBase> & prototype) {
                                             return prototype->name() == name;
                                         });

                if (iter == prototypes.end()) continue;

                Assert(!foundBaseEntityPrototype, "Multiple bases for component '" + name + "' found");

                auto & baseProtoype = *iter;

                /**
                 * Merge JSONs
                 */
                Json mergedJson = (*iter)->json();

                for (auto & pair : Json::iterator_wrapper(componentPrototype->json()))
                {
                    mergedJson[pair.key()] = pair.value();
                }

                componentPrototype->setJson(mergedJson);

                foundBaseEntityPrototype = true;
            }
        }
    }

    /**
     *
     */
    for (auto & entityPrototype : m_entityPrototypesInOrder)
    {
        entityPrototype->updateEntities();
    }
}

Entity PrototypeManager::createEntity(const std::string & prototypeKey, const std::string & entityName)
{
    auto iter = m_entityPrototypeByKey.find(prototypeKey);
    Assert(iter != m_entityPrototypeByKey.end(), "Couldn't find '" + prototypeKey + "'");

    return iter->second->createEntity(entityName.empty() ? "Unnamed " + prototypeKey : entityName);
}

void PrototypeManager::updateEntities()
{
    updateEntityPrototypeOrder();
    for (auto & entityPrototype : m_entityPrototypesInOrder)
    {
        entityPrototype->updateEntities();
    }
}

void PrototypeManager::updateEntityPrototypeOrder()
{

    /**
     * Update EntityPrototype orders
     */
    {
        for (auto & pair : m_entityPrototypeByKey)
        {
            pair.second->setOrder(std::numeric_limits<u32>::max());
        }

        auto currentOrder = u32{0};

        for (size_t numUpdatedEntityProtoypes = 0; numUpdatedEntityProtoypes < m_entityPrototypeByKey.size(); )
        {
            for (auto & pair : m_entityPrototypeByKey)
            {
                auto & entityPrototype = pair.second;

                if (entityPrototype->order() < currentOrder) continue;

                auto entityPrototypeIsOfCurrentOrder = std::all_of(entityPrototype->baseEntityPrototypes().begin(),
                                                                   entityPrototype->baseEntityPrototypes().end(),
                                                                   [&] (const std::shared_ptr<EntityPrototype> & base) {
                                                                       return base->order() < currentOrder;
                                                                   });

                if (entityPrototypeIsOfCurrentOrder)
                {
                    entityPrototype->setOrder(currentOrder);
                    numUpdatedEntityProtoypes++;
                }
            }
            currentOrder++;
        }
    }

    /**
     * Create ordered vector of EntityPrototypes
     */
    m_entityPrototypesInOrder.clear();
    m_entityPrototypesInOrder.reserve(m_entityPrototypeByKey.size());

    for (auto & pair : m_entityPrototypeByKey)
    {
        m_entityPrototypesInOrder.emplace_back(pair.second);
    }

    std::sort(m_entityPrototypesInOrder.begin(), m_entityPrototypesInOrder.end(), [](
        const std::shared_ptr<EntityPrototype> & l,
        const std::shared_ptr<EntityPrototype> & r) {
        return l->order() < r->order();
    });
}

}