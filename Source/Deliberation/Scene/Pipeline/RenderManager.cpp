#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include <imgui.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Scene/Debug/DebugSurfaceOverlayRenderer.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>
#include <Deliberation/Scene/Util/RangedGpuScope.h>

namespace deliberation
{
RenderManager::RenderManager(DrawContext & drawContext)
    : m_drawContext(drawContext)
{
    m_backbufferClear = m_drawContext.createClear();

    m_surfaceOverlayRenderer = addRenderer<DebugSurfaceOverlayRenderer>();
}

void RenderManager::registerRenderNode(
    const std::shared_ptr<RenderNode> & node, const RenderPhase & phase)
{
    m_renderNodesByPhase[(size_t)phase].emplace_back(node);
}

void RenderManager::render()
{
    if (!m_pipelineBuild)
    {
        const auto w = m_drawContext.backbuffer().width();
        const auto h = m_drawContext.backbuffer().height();

        m_mainCamera.setAspectRatio((float)w / (float)h);

        // Setup GBuffer
        FramebufferDesc gbufferDesc(
            w,
            h,
            {{PixelFormat_RGB_32_F, "Diffuse"},
             {PixelFormat_RGB_32_F, "Position"},
             {PixelFormat_RGB_32_F, "Normal"}},
            {{PixelFormat_Depth_16_UN}});

        m_gbuffer = m_drawContext.createFramebuffer(gbufferDesc);
        m_gbuffer.clear().setColor(
            "Position",
            glm::vec4(0.0f, 0.0f, -std::numeric_limits<float>::max(), 0.0f));

        FramebufferDesc hdrBufferDesc(w, h, {{PixelFormat_RGB_32_F, "Hdr"}});
        m_hdrBuffer = m_drawContext.createFramebuffer(hdrBufferDesc);

        FramebufferDesc ssaoBufferDesc(w, h, {{PixelFormat_R_32_F, "Ssao"}});
        m_ssaoBuffer = m_drawContext.createFramebuffer(ssaoBufferDesc);

        m_renderNodesByPhase.clear();
        for (auto & renderer : m_renderers)
        {
            renderer->registerRenderNodes();
        }

        m_pipelineBuild = true;
    }



    // Clear
    m_backbufferClear.render();
    m_gbuffer.clear().render();

    // Render Phases
    for (auto & pair : m_renderNodesByPhase)
    {
        auto & nodesInPhase = pair.second;

        if (!nodesInPhase.empty())
        {
            RangedGpuScope gpuScope1("Phase - " + RenderPhaseToString((RenderPhase)pair.first));

            for (auto & node : pair.second)
            {
                RangedGpuScope gpuScope2("Node - " + node->name());
                node->render();
            }
        }
    }
}
}