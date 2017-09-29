#pragma once

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{
class SingleNodeRenderer : public Renderer
{
public:
    SingleNodeRenderer(
        RenderPhase         renderPhase,
        const std::string & name = {"Unnamed Renderer"})
        : Renderer(name), m_renderPhase(renderPhase)
    {
    }

    void onRegisterRenderNodes() override
    {
        class SingleRenderNode : public RenderNode
        {
        public:
            SingleRenderNode(SingleNodeRenderer & renderer)
                : RenderNode(renderer.shared_from_this()), m_renderer(renderer)
            {
            }

            void        render() override { m_renderer.render(); }
            std::string name() const override { return m_renderer.name(); }

            SingleNodeRenderer & m_renderer;
        };

        GetGlobal<RenderManager>()->registerRenderNode(
            std::make_shared<SingleRenderNode>(*this), m_renderPhase);
        onSetupRender();
    }

    virtual void render() = 0;

protected:
    virtual void onSetupRender() {};

private:
    RenderPhase m_renderPhase;
};
}