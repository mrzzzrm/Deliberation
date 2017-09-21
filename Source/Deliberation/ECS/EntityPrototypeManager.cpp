#include <Deliberation/ECS/EntityPrototypeManager.h>

#include <fstream>
#include <limits>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/FilesystemUtils.h>
#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/Json.h>

#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Platform/Application.h>
#include <Deliberation/Platform/ApplicationRuntime.h>

namespace deliberation
{
EntityPrototypeManager::EntityPrototypeManager(const std::shared_ptr<World> & world, const std::string & listPath)
    : m_world(world), m_listPoll(listPath)
{
}

const std::shared_ptr<EntityPrototype> &
EntityPrototypeManager::getOrCreateEntityPrototype(const std::string & key)
{
    DELIBERATION_LOG_INNER_SCOPE("EntityPrototypeManager");

    auto iter = m_entityPrototypeByKey.find(key);

    if (iter == m_entityPrototypeByKey.end())
    {
        Log->info("Registering EntityPrototype '{}'", key);

        iter =
            m_entityPrototypeByKey
                .emplace(key, std::make_shared<EntityPrototype>(key))
                .first;
    }

    return iter->second;
}

std::shared_ptr<ComponentPrototypeBase>
EntityPrototypeManager::getOrAddComponentPrototype(
    const std::shared_ptr<EntityPrototype> & entityPrototype,
    const std::string &                      componentPrototypeName)
{
    auto & componentPrototypes = entityPrototype->componentPrototypes();

    const auto iter = std::find_if(
        componentPrototypes.begin(),
        componentPrototypes.end(),
        [&](const std::shared_ptr<ComponentPrototypeBase> &
                componentPrototype) {
            return componentPrototypeName == componentPrototype->name();
        });

    if (iter == componentPrototypes.end())
    {
        auto iter2 =
            m_componentPrototypeFactoryByName.find(componentPrototypeName);
        AssertM(
            iter2 != m_componentPrototypeFactoryByName.end(),
            "ComponentPrototype '" + componentPrototypeName +
                "' not registered");

        auto componentPrototype = iter2->second();
        entityPrototype->addComponentPrototype(componentPrototype);

        return componentPrototype;
    }

    return *iter;
}

void EntityPrototypeManager::reloadList()
{
    DELIBERATION_LOG_INNER_SCOPE("EntityPrototypeManager");

    if (m_listPoll.path().empty()) return;

    /**
     * TODO Move to <filesystem> as soon as it becomes widely available
     */

    const auto listDir = GetDirFromPath(m_listPoll.path());

    std::ifstream listFile(m_listPoll.path());
    AssertM(
        listFile.is_open(),
        "Failed to open prototype list '" + m_listPoll.path() + "'");

    Json listJson;
    listFile >> listJson;

    /**
     * Make sure all EntityPrototypes exist
     */
    for (const auto & prototypeName : listJson["Prototypes"])
    {
        auto entityPrototype = getOrCreateEntityPrototype(prototypeName);
    }

    /**
     * Load EntityPrototype JSONs
     */
    for (const auto & prototypeName : listJson["Prototypes"])
    {
        auto entityPrototype = getOrCreateEntityPrototype(prototypeName);

        const auto prototypePath =
            listDir + prototypeName.get<std::string>() + ".json";

        auto iter = m_entityPrototypeFilePollsByPath.find(prototypePath);
        if (iter == m_entityPrototypeFilePollsByPath.end())
        {
            iter = m_entityPrototypeFilePollsByPath
                       .emplace(prototypePath, prototypePath)
                       .first;
        }
        if (!iter->second.check()) continue;

        Log->info("Loading EntityPrototype '{}' from '{}'", prototypeName.get<std::string>(), prototypePath);

        std::ifstream prototypeFile(prototypePath);
        AssertM(
            prototypeFile.is_open(),
            "Failed to open prototype '" + prototypePath + "'");

        Json prototypeJson;
        prototypeFile >> prototypeJson;

        entityPrototype->setJson(prototypeJson);
    }
}

Entity EntityPrototypeManager::createEntity(
    const std::string & prototypeKey, const std::string & entityName)
{
    return createEntity(std::vector<std::string>{prototypeKey}, entityName);
}

Entity EntityPrototypeManager::createEntity(
    const std::vector<std::string> & prototypeKeys,
    const std::string &              entityName)
{
    DELIBERATION_LOG_INNER_SCOPE("EntityPrototypeManager");

    //Log->info("Creating '{}' from ''", entityName, prototypeKeys);

    std::cout << "EntityPrototypeManager: Getting World = " << &m_world << std::endl;

    auto entity = Application::get().runtime()->world()->createEntity(
        entityName.empty() ? "Unnamed Entity" : entityName);

    for (const auto & prototypeKey : prototypeKeys)
    {
        auto iter = m_entityPrototypeByKey.find(prototypeKey);
        AssertM(
            iter != m_entityPrototypeByKey.end(),
            "Couldn't find '" + prototypeKey + "'");

        iter->second->applyToEntity(entity);
    }

    return entity;
}

void EntityPrototypeManager::updateEntities()
{
    /**
     * Update Derivate->Base relations and ComponentPrototype JSONs
     */
    for (auto & pair : m_entityPrototypeByKey)
    {
        auto &       entityPrototype = pair.second;
        const auto & entityPrototypeJson = entityPrototype->json();

        /**
         * Derivate->Base relations...
         */
        if (entityPrototypeJson.count("Bases") > 0)
        {
            for (auto & baseEntityPrototypeName : entityPrototypeJson["Bases"])
            {
                auto & baseEntityPrototypes =
                    entityPrototype->baseEntityPrototypes();
                const auto iter = std::find_if(
                    baseEntityPrototypes.begin(),
                    baseEntityPrototypes.end(),
                    [&](const std::shared_ptr<EntityPrototype> &
                            exisitingBaseEntityPrototype) {
                        return baseEntityPrototypeName ==
                               exisitingBaseEntityPrototype->key();
                    });

                if (iter == baseEntityPrototypes.end())
                {
                    auto baseEntityPrototype =
                        getOrCreateEntityPrototype(baseEntityPrototypeName);
                    entityPrototype->addBaseEntityPrototype(
                        baseEntityPrototype);
                }
            }
        }

        /**
         * ...and ComponentPrototype JSONs
         */
        if (entityPrototypeJson.count("Components") > 0)
        {
            for (auto & pair :
                 Json::iterator_wrapper(entityPrototypeJson["Components"]))
            {
                auto componentPrototypeName = pair.key();
                auto componentPrototype = getOrAddComponentPrototype(
                    entityPrototype, componentPrototypeName);
                // Patch existing Json!?
                componentPrototype->setNewJson(pair.value());
            }
        }
    }

    /**
     * Update EntityPrototype Order
     */
    for (auto & pair : m_entityPrototypeByKey)
    {
        pair.second->setOrder(std::numeric_limits<u32>::max());
    }

    auto currentOrder = u32{0};

    for (size_t numUpdatedEntityProtoypes = 0;
         numUpdatedEntityProtoypes < m_entityPrototypeByKey.size();)
    {
        for (auto & pair : m_entityPrototypeByKey)
        {
            auto & entityPrototype = pair.second;

            if (entityPrototype->order() < currentOrder) continue;

            auto entityPrototypeIsOfCurrentOrder = std::all_of(
                entityPrototype->baseEntityPrototypes().begin(),
                entityPrototype->baseEntityPrototypes().end(),
                [&](const std::shared_ptr<EntityPrototype> & base) {
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

    /**
     * Create ordered vector of EntityPrototypes
     */
    m_entityPrototypesInOrder.clear();
    m_entityPrototypesInOrder.reserve(m_entityPrototypeByKey.size());

    for (auto & pair : m_entityPrototypeByKey)
    {
        m_entityPrototypesInOrder.emplace_back(pair.second);
    }

    std::sort(
        m_entityPrototypesInOrder.begin(),
        m_entityPrototypesInOrder.end(),
        [](const std::shared_ptr<EntityPrototype> & l,
           const std::shared_ptr<EntityPrototype> & r) {
            return l->order() < r->order();
        });

    /**
     * Propagate ComponentPrototypes from Bases to Derivates
     * Patch ComponentPrototypes JSON with values from derived
     * ComponentPrototype
     */
    for (auto & entityPrototype : m_entityPrototypesInOrder)
    {
        for (const auto & baseEntityPrototype :
             entityPrototype->baseEntityPrototypes())
        {
            for (const auto & baseComponentPrototype :
                 baseEntityPrototype->componentPrototypes())
            {
                getOrAddComponentPrototype(
                    entityPrototype, baseComponentPrototype->name());
            }
        }

        for (auto & componentPrototype : entityPrototype->componentPrototypes())
        {
            auto & name = componentPrototype->name();

            auto foundBaseEntityPrototype = false;

            for (auto & baseEntityPrototype :
                 entityPrototype->baseEntityPrototypes())
            {
                auto & prototypes = baseEntityPrototype->componentPrototypes();

                const auto iter = std::find_if(
                    prototypes.begin(),
                    prototypes.end(),
                    [&](const std::shared_ptr<ComponentPrototypeBase> &
                            prototype) { return prototype->name() == name; });

                if (iter == prototypes.end()) continue;

                AssertM(
                    !foundBaseEntityPrototype,
                    "Multiple bases for component '" + name + "' found");

                auto & basePrototype = *iter;

                componentPrototype->setNewJson(mergeJson(
                    componentPrototype->newJson(), basePrototype->newJson()));

                foundBaseEntityPrototype = true;
            }
        }
    }

    /**
     * Update Entities
     */
    for (auto & entityPrototype : m_entityPrototypesInOrder)
    {
        entityPrototype->updateEntities();
    }
}

Json EntityPrototypeManager::mergeJson(const Json & a, const Json & b)
{
    Json mergedJson = b;

    if (!a.empty())
    {
        for (auto & pair : Json::iterator_wrapper(a))
        {
            mergedJson[pair.key()] = pair.value();
        }
    }

    return mergedJson;
}
}