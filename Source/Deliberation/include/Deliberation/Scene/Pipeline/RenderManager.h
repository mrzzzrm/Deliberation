#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/LinearMap.h>

#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/Scene/Pipeline/RenderPhase.h>
#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{

class DrawContext;
class Renderer;
class RenderNode;

class RenderManager
{
public:
    RenderManager(DrawContext & drawContext);
    virtual ~RenderManager() = default;

    DrawContext & drawContext() { return m_drawContext; }
    Camera3D & mainCamera() { return m_mainCamera; }
    Framebuffer & gbuffer() { return m_gbuffer; }
    Framebuffer & hdrBuffer() { return m_hdrBuffer; }
    Framebuffer & ssaoBuffer() { return m_ssaoBuffer; }

    template<typename T, typename ... Args>
    std::shared_ptr<T> addRenderer(Args &&... args);

    // Callbacks for Renderers
    void registerRenderNode(const std::shared_ptr<RenderNode> & node, const RenderPhase & phase);

    void render();

private:
    DrawContext &                                       m_drawContext;
    Camera3D                                            m_mainCamera;
    std::vector<std::shared_ptr<Renderer>>              m_renderers;
    bool                                                m_pipelineBuild = false;
    LinearMap<std::vector<std::shared_ptr<RenderNode>>> m_renderNodesByPhase;
    Framebuffer                                         m_gbuffer;
    Framebuffer                                         m_hdrBuffer;
    Framebuffer                                         m_ssaoBuffer;
};

}

#include <Deliberation/Scene/Pipeline/RenderManager.inl>