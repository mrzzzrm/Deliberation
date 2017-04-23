#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/LinearMap.h>

#include <Deliberation/Scene/Pipeline/RenderPhase.h>

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

    // Callbacks for Renderers
    void registerRenderer(const std::shared_ptr<Renderer> & renderer);
    void registerRenderNode(const std::shared_ptr<RenderNode> & node, const RenderPhase & phase);

    void render();

private:
    DrawContext &                                       m_drawContext;
    std::vector<std::shared_ptr<Renderer>>              m_renderers;
    bool                                                m_pipelineBuild = false;
    LinearMap<std::vector<std::shared_ptr<RenderNode>>> m_renderNodesByPhase;
};

}