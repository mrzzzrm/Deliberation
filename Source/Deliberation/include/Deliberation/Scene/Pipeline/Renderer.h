#pragma once

namespace deliberation
{

class RenderManager;

class Renderer
{
public:
    Renderer(RenderManager & renderManager): m_renderManager(renderManager) {}
    virtual ~Renderer() = default;

    virtual std::shared_ptr<RenderNode> createRenderNode(const RenderPhase & renderPhase,
                                                         const RenderView & renderView) = 0;

protected:
    RenderManager & m_renderManager;
};

}