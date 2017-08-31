#include <typeinfo>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{
template<typename T, typename... Args>
std::shared_ptr<T> RenderManager::addRenderer(Args &&... args)
{
    auto renderer = std::make_shared<T>(*this, std::forward<Args>(args)...);
    m_renderers.emplace_back(renderer);
    m_rendererByTypeId.emplace(TypeID::value<RenderManager, T>(), renderer);
    m_pipelineBuild = false;

    renderer->onCreated();

    return renderer;
}

template<typename RendererType>
std::shared_ptr<RendererType> RenderManager::renderer()
{
    const auto iter = m_rendererByTypeId.find(TypeID::value<RenderManager, RendererType>());
    AssertM(iter != m_rendererByTypeId.end(), std::string("Renderer '") + typeid(RendererType).name() + "' not registered");
    return std::static_pointer_cast<RendererType>(iter->second);
}

template<typename RendererType>
std::shared_ptr<const RendererType> RenderManager::renderer() const
{
    const auto iter = m_rendererByTypeId.find(TypeID::value<RenderManager, RendererType>());
    AssertM(iter != m_rendererByTypeId.end(), std::string("Renderer '") + typeid(RendererType).name() + "' not registered");
    return std::static_pointer_cast<RendererType>(iter->second);
}
}