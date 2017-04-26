#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Pipeline/Renderer.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

namespace deliberation
{

RenderManager::RenderManager(DrawContext & drawContext):
    m_drawContext(drawContext)
{}

void RenderManager::registerRenderNode(const std::shared_ptr<RenderNode> & node, const RenderPhase & phase)
{
    m_renderNodesByPhase[(size_t)phase].emplace_back(node);
}

void RenderManager::render()
{
    if (!m_pipelineBuild)
    {
        const auto w = m_drawContext.backbuffer().width();
        const auto h = m_drawContext.backbuffer().height();

        m_gbuffer = m_drawContext.createFramebuffer(w, h);
        m_gbuffer.addDepthTarget(PixelFormat_Depth_16_UN);
        m_gbuffer.addRenderTarget(PixelFormat_RGB_32_F); // Diffuse
        m_gbuffer.addRenderTarget(PixelFormat_RGB_32_F); // Position
        m_gbuffer.clear().setColor(1, glm::vec4(0.0f, 0.0f, -std::numeric_limits<float>::max(), 0.0f));
        m_gbuffer.addRenderTarget(PixelFormat_RGB_32_F); // Normal


        m_hdrBuffer = m_drawContext.createFramebuffer(w, h);
        m_hdrBuffer.addRenderTarget(PixelFormat_RGB_32_F);

        m_ssaoBuffer = m_drawContext.createFramebuffer(w, h);
        m_ssaoBuffer.addRenderTarget(PixelFormat_R_32_F);

        m_renderNodesByPhase.clear();
        for (auto & renderer : m_renderers)
        {
            renderer->registerRenderNodes();
        }

        m_pipelineBuild = true;
    }

    m_gbuffer.clear().render();

    for (auto & pair : m_renderNodesByPhase)
    {
        for (auto & node : pair.second)
        {
            node->render();
        }
    }
}

}