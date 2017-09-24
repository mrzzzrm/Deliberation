#pragma once

#include <memory>

#include <Deliberation/Core/HashUtils.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Resource/ResourceToken.h>

#include <Deliberation/Scene/MeshData.h>

namespace deliberation
{
class DrawContext;

// Legacy, remove ASAP
enum class R
{
    None,
    UiCrosshairMesh,
    HudEntityMarkerUpperLeft,
    HudElement
};

using LegacyResourceId = R;

class ResourceManager
{
public:
    template<typename ResourceType> using ResourceLoader = std::function<ResourceType(const std::string &)>;

public:
    ResourceManager();

    // Old shit
    const MeshData & mesh(LegacyResourceId resourceId) const;
    const Program &  program(LegacyResourceId resourceId) const;


    //
    template<typename ResourceType>
        void setLoader(const ResourceLoader<ResourceType> & loader);

    template<typename ResourceType>
        ResourceToken resourceToken(const std::string & path);

    template<typename ResourceType>
        const ResourceType & resource(const std::string & path);

    template<typename ResourceType>
        const ResourceType & resource(const ResourceToken & token);

    template<typename ResourceType>
        void addResource(const std::string & path, ResourceType && resource);

    // Legacy, remove ASAP
    void registerBuildIns();

private:
    struct AbstractResourceContainer
    {
        virtual ~AbstractResourceContainer() = default;
    };

    template<typename ResourceType>
    struct ResourceContainer : AbstractResourceContainer
    {
        explicit ResourceContainer(const ResourceLoader<ResourceType> & loader):
            loader(loader)
        {}

        deliberation::ResourceId addResource(const std::string & path, ResourceType && resource);
        deliberation::ResourceId getOrLoadResource(const std::string & path);

        deliberation::ResourceId idCounter = 0;
        ResourceLoader<ResourceType> loader;
        std::unordered_map<deliberation::ResourceId, ResourceType> resourceById;
        std::unordered_map<std::string, deliberation::ResourceId> resourceIdByPath;
    };

private:
    template<typename ResourceType>
    ResourceContainer<ResourceType> & containerRef();

private:
    // Old shit
    std::unordered_map<size_t, std::shared_ptr<MeshData>> m_meshByResourceId;
    std::unordered_map<size_t, Program>                   m_programByResourceId;

    //
    std::unordered_map<TypeID::value_t, std::shared_ptr<AbstractResourceContainer>>
        m_resourceContainersByTypeID;
};

}

#include <Deliberation/Resource/ResourceManager.inl>