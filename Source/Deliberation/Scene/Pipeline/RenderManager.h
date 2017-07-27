#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/LinearMap.h>

#include <Deliberation/Draw/Framebuffer.h>
#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>

namespace deliberation
{
class DebugSurfaceOverlayRenderer;
class DrawContext;
class Renderer;
class RenderNode;

class RenderManager
{
public:
    struct RegisteredFramebuffer
    {
        Framebuffer framebuffer;
        std::string name;
    };

public:
    RenderManager(DrawContext & drawContext);
    virtual ~RenderManager() = default;

    DrawContext & drawContext() { return m_drawContext; }
    Camera3D &    mainCamera() { return m_mainCamera; }
    const std::vector<std::shared_ptr<Renderer>> & renderers() { return m_renderers; }
    template<typename RendererType> std::shared_ptr<RendererType> renderer();
    template<typename RendererType> std::shared_ptr<const RendererType> renderer() const;
    Framebuffer &                 gbuffer() { return m_gbuffer; }
    Framebuffer &                 hdrBuffer() { return m_hdrBuffer; }
    Framebuffer &                 ssaoBuffer() { return m_ssaoBuffer; }
    DebugSurfaceOverlayRenderer & surfaceOverlayRenderer()
    {
        return *m_surfaceOverlayRenderer;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> addRenderer(Args &&... args);

    // Callbacks for Renderers
    void registerRenderNode(
        const std::shared_ptr<RenderNode> & node, const RenderPhase & phase);

    void registerFramebuffer(const Framebuffer & framebuffer)
    {
        m_framebuffers.emplace_back(framebuffer);
    }
    const std::vector<Framebuffer> & framebuffers() const
    {
        return m_framebuffers;
    }

    void render();

private:
    DrawContext &                                       m_drawContext;
    Camera3D                                            m_mainCamera;
    std::vector<std::shared_ptr<Renderer>>              m_renderers;
    std::unordered_map<TypeID::value_t, std::shared_ptr<Renderer>>
                                                        m_rendererByTypeId;
    bool                                                m_pipelineBuild = false;
    LinearMap<std::vector<std::shared_ptr<RenderNode>>> m_renderNodesByPhase;
    Framebuffer                                         m_gbuffer;
    Framebuffer                                         m_hdrBuffer;
    Framebuffer                                         m_ssaoBuffer;
    Clear                                               m_backbufferClear;
    std::shared_ptr<DebugSurfaceOverlayRenderer>        m_surfaceOverlayRenderer;

    // For Debugging
    std::vector<Framebuffer> m_framebuffers;
};
}

#include <Deliberation/Scene/Pipeline/RenderManager.inl>