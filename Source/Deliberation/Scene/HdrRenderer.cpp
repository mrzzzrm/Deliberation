#include <Deliberation/Scene/HdrRenderer.h>

#include <Deliberation/Draw/DepthState.h>
#include <Deliberation/Draw/DrawState.h>

#include <Deliberation/Scene/ScreenSpaceEffect.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

namespace deliberation
{
class HdrNode : public RenderNode
{
  public:
    HdrNode(HdrRenderer & hdrRenderer) : RenderNode(hdrRenderer.renderManager())
    {
        m_effect = ScreenSpaceEffect(
            m_renderManager.drawContext(),
            {DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
             DeliberationDataPath("Data/Shaders/Hdr.frag")},
            "HDR");
        m_effect.draw().sampler("Hdr").setTexture(
            m_renderManager.hdrBuffer().colorTargetRef("Hdr"));
        m_effect.draw().sampler("Depth").setTexture(
            m_renderManager.gbuffer().depthTargetRef());
        m_effect.draw().state().setDepthState(DepthState::enabled());
    }

    void render() override { m_effect.render(); }

  private:
    ScreenSpaceEffect m_effect;
};

HdrRenderer::HdrRenderer(RenderManager & renderManager)
    : Renderer(renderManager)
{
}

void HdrRenderer::registerRenderNodes()
{
    m_renderManager.registerRenderNode(
        std::make_shared<HdrNode>(*this), RenderPhase::Hdr);
}
}