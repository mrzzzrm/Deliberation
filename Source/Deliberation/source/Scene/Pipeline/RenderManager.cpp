#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include <Deliberation/Scene/Pipeline/Renderer.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

namespace deliberation
{

RenderManager::RenderManager(DrawContext & drawContext):
    m_drawContext(drawContext)
{}

void RenderManager::registerRenderer(const std::shared_ptr<Renderer> & renderer)
{
    m_renderers.emplace_back(renderer);
}

void RenderManager::registerRenderNode(const std::shared_ptr<RenderNode> & node, const RenderPhase & phase)
{
    m_renderNodesByPhase[phase].emplace_back(node);
}

void RenderManager::render()
{
    if (!m_pipelineBuild)
    {
        m_renderNodesByPhase.clear();
        for (auto & renderer : m_renderers)
        {
            renderer->registerRenderNodes();
        }

        m_pipelineBuild = true;
    }

    for (auto & pair : m_renderNodesByPhase)
    {
        for (auto & node : pair.second)
        {
            node->render();
        }
    }
}

}