#pragma once

#include <Deliberation/Scene/Pipeline/Renderer.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>

namespace deliberation
{

class SingleNodeRenderer:
    public Renderer
{
public:
    SingleNodeRenderer(RenderManager & renderManager, RenderPhase renderPhase):
        Renderer(renderManager),
        m_renderPhase(renderPhase)
    {

    }

    void registerRenderNodes() override
    {
        class SingleRenderNode:
            public RenderNode
        {
        public:
            SingleRenderNode(SingleNodeRenderer & renderer):
                RenderNode(renderer.renderManager()),
                m_renderer(renderer)
            {}

            void render() override
            {
                m_renderer.render();
            }

            SingleNodeRenderer & m_renderer;
        };

        m_renderManager.registerRenderNode(std::make_shared<SingleRenderNode>(*this), m_renderPhase);
    }

    virtual void render() = 0;

private:
    RenderPhase m_renderPhase;
};

}