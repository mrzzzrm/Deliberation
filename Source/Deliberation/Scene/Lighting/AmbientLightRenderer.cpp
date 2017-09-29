#include <Deliberation/Scene/Lighting/AmbientLightRenderer.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{
class AmbientLightNode : public RenderNode
{
public:
    AmbientLightNode(AmbientLightRenderer & renderer)
        : m_renderer(renderer), RenderNode(renderer.shared_from_this())
    {
        m_effect = ScreenSpaceEffect(
            {DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
             DeliberationDataPath("Data/Shaders/AmbientLight.frag")},
            "AmbientLight");

        m_effect.draw().setFramebuffer(GetGlobal<RenderManager>()->hdrBuffer());
        m_effect.draw().sampler("Ssao").setTexture(
            GetGlobal<RenderManager>()->ssaoBuffer().colorTargetRef("Ssao"));
        m_effect.draw().sampler("Diffuse").setTexture(
            GetGlobal<RenderManager>()->gbuffer().colorTargetRef("Diffuse"));
        m_effect.draw().state().setBlendState(
            {BlendEquation::Add, BlendFactor::One, BlendFactor::One});
    }

    void render() override { m_effect.render(); }

private:
    AmbientLightRenderer & m_renderer;
    ScreenSpaceEffect      m_effect;
};

AmbientLightRenderer::AmbientLightRenderer()
    : Renderer("AmbientLight")
{
}

void AmbientLightRenderer::onRegisterRenderNodes()
{
    GetGlobal<RenderManager>()->registerRenderNode(
        std::make_shared<AmbientLightNode>(*this), RenderPhase::Lighting);
}
}