#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include <imgui.h>

#include <Deliberation/Core/Scope.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Draw/Util/RangedGpuScope.h>
#include <Deliberation/Scene/Debug/DebugSurfaceOverlayRenderer.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{
DELIBERATION_DEFINE_GLOBAL(RenderManager);

RenderManager::RenderManager()
{
    m_backbufferClear = GetGlobal<DrawContext>()->createClear();
    m_mainCamera.setPosition({0.0f, 0.0f, 100.0f});

    m_surfaceOverlayRenderer = addRenderer<DebugSurfaceOverlayRenderer>();
}

void RenderManager::registerRenderNode(
    const std::shared_ptr<RenderNode> & node, const RenderPhase & phase)
{
    m_renderNodesByPhase[(size_t)phase].emplace_back(node);
}

void RenderManager::render()
{
    DELIBERATION_SCOPE("RenderManager::render()");

    if (!m_pipelineBuild)
    {
        const auto w = GetGlobal<DrawContext>()->backbuffer().width();
        const auto h = GetGlobal<DrawContext>()->backbuffer().height();

        m_mainCamera.setAspectRatio((float)w / (float)h);

        // Setup GBuffer
        FramebufferDesc gbufferDesc(
            w,
            h,
            {{PixelFormat_RGB_32_F, "Diffuse"},
             {PixelFormat_RGB_32_F, "Position"},
             {PixelFormat_RGB_32_F, "Normal"}},
            {{PixelFormat_Depth_24_UN}},
            "GBuffer");

        m_gbuffer = GetGlobal<DrawContext>()->createFramebuffer(gbufferDesc);
        m_gbuffer.clear().setColor(
            "Position",
            glm::vec4(0.0f, 0.0f, -std::numeric_limits<float>::max(), 0.0f));

        FramebufferDesc hdrBufferDesc(
            w,
            h,
            {{PixelFormat_RGB_32_F, "Hdr"}},
            {{m_gbuffer.depthTargetRef()}},
            "HDR");
        m_hdrBuffer = GetGlobal<DrawContext>()->createFramebuffer(hdrBufferDesc);

        FramebufferDesc ssaoBufferDesc(
            w, h, {{PixelFormat_R_32_F, "Ssao"}}, {}, "SSAO");
        m_ssaoBuffer = GetGlobal<DrawContext>()->createFramebuffer(ssaoBufferDesc);

        m_renderNodesByPhase.clear();
        for (auto & renderer : m_renderers)
        {
            renderer->onRegisterRenderNodes();
        }

        registerFramebuffer(m_ssaoBuffer);
        registerFramebuffer(m_gbuffer);
        registerFramebuffer(m_hdrBuffer);

        m_pipelineBuild = true;
    }

    // Clear
    m_backbufferClear.render();
    m_gbuffer.clear().render();
    m_hdrBuffer.clear().render();

    for (auto & renderer : m_renderers)
    {
        renderer->onBeforeRender();
    }

    // Render Phases
    for (auto & pair : m_renderNodesByPhase)
    {
        auto & nodesInPhase = pair.second;

        if (!nodesInPhase.empty())
        {
            RangedGpuScope gpuScope1(
                "Phase - " + RenderPhaseToString((RenderPhase)pair.first));

            for (auto & node : pair.second)
            {
                RangedGpuScope gpuScope2("Node - " + node->name());

                auto renderer = node->renderer();
                AssertM((bool)renderer, "");

                if (renderer->enabled() && node->enabled())
                {
                    node->render();
                }
            }
        }
    }
}
}