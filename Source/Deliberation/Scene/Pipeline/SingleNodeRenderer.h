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
        RenderManager &     renderManager,
        RenderPhase         renderPhase,
        const std::string & name = {"Unnamed Renderer"})
        : Renderer(renderManager, name), m_renderPhase(renderPhase)
    {
    }

    void registerRenderNodes() override
    {
        class SingleRenderNode : public RenderNode
        {
          public:
            SingleRenderNode(SingleNodeRenderer & renderer)
                : RenderNode(renderer.renderManager()), m_renderer(renderer)
            {
            }

            void render() override { m_renderer.render(); }

            SingleNodeRenderer & m_renderer;
        };

        m_renderManager.registerRenderNode(
            std::make_shared<SingleRenderNode>(*this), m_renderPhase);
        onSetupRender();
    }

    virtual void render() = 0;

  protected:
    virtual void onSetupRender(){};

  private:
    RenderPhase m_renderPhase;
};
}