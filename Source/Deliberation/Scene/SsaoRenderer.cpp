#include <Deliberation/Scene/SsaoRenderer.h>

#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/Core/Math/MathUtils.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/TextureLoader.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class SsaoNode:
    public RenderNode
{
public:
    SsaoNode(SsaoRenderer & ssaoRenderer):
        RenderNode(ssaoRenderer.renderManager())
    {
        auto & drawContext = m_renderManager.drawContext();

        m_intermediateFb = drawContext.createFramebuffer(drawContext.backbuffer().width(), drawContext.backbuffer().height());
        m_intermediateFb.addRenderTarget(PixelFormat_R_32_F);

        m_effect = ScreenSpaceEffect(m_renderManager.drawContext(), {DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
                                                                     DeliberationDataPath("Data/Shaders/Ssao.frag")}, "SSAO");

        auto positionSampler = m_effect.draw().sampler("Position");
        positionSampler.setTexture(m_renderManager.gbuffer().renderTarget(1)->texture());
        positionSampler.setWrap(gl::GL_CLAMP_TO_EDGE);

        auto normalSampler = m_effect.draw().sampler("Normal");
        normalSampler.setTexture(m_renderManager.gbuffer().renderTarget(2)->texture());
        normalSampler.setWrap(gl::GL_CLAMP_TO_EDGE);

        m_effect.draw().setFramebuffer(m_intermediateFb);

        const auto & interface = m_effect.draw().program().interface();

        // Samples
        const auto numSamples = std::min<int>(5, interface.uniform("Samples")->arraySize());
        m_samples = LayoutedBlob({"Sample", Type_Vec3}, numSamples);
        auto samples = m_samples.iterator<glm::vec3>("Sample");

        for (size_t s = 0; s < numSamples; s++)
        {
            auto sample = RandomOnHemisphere({0.0f, 0.0f, 1.0f});
            auto scale = (float)s / numSamples;
            scale = glm::mix(0.1f, 1.0f, scale * scale);

            samples.put(sample * scale);
        }

        m_effect.draw().uniform("Samples").set(m_samples);

        // Noise Texture
        auto noiseWidth = 4u;
        auto noiseHeight = 4u;
        auto numNoiseTexels = noiseWidth * noiseHeight;

        LayoutedBlob noiseData({"Noise", Type_Vec3}, numNoiseTexels);
        auto noise = noiseData.iterator<glm::vec3>("Noise");

        for (size_t i = 0; i < numNoiseTexels; i++)
        {
            noise.put({RandomFloat(-1.0f, 1.0f),
                  RandomFloat(-1.0f, 1.0f),
                  0.0f});
        }

        auto noiseTexture = drawContext.createTexture(
            TextureLoader(noiseData.rawData(), noiseWidth, noiseHeight, PixelFormat_RGB_32_F).load());

        auto noiseSampler = m_effect.draw().sampler("Noise");
        noiseSampler.setTexture(noiseTexture);
        noiseSampler.setWrapS(gl::GL_REPEAT);
        noiseSampler.setWrapT(gl::GL_REPEAT);

        // NoiseScale
        const auto noiseScaleX = (float)drawContext.backbuffer().width() / (float)noiseWidth;
        const auto noiseScaleY = (float)drawContext.backbuffer().height() / (float)noiseHeight;
        m_effect.draw().uniform("NoiseScale").set(glm::vec2(noiseScaleX, noiseScaleY));

        // Etc uniforms
        m_effect.draw().uniform("Radius").set(0.3f);
        m_effect.draw().uniform("Bias").set(0.02f);
        m_effect.draw().uniform("NoiseEnabled").set(true);
        m_effect.draw().uniform("NumSamples").set(numSamples);
        m_projectionUniform = m_effect.draw().uniform("Projection");

        // Blur
        m_blurEffect = ScreenSpaceEffect(drawContext, {DeliberationDataPath("Data/Shaders/Blur4x4.frag"),
             DeliberationDataPath("Data/Shaders/UV_Position2.vert")}, "SSAO Blur");

        auto inputSampler = m_blurEffect.draw().sampler("Input");
        inputSampler.setTexture(m_intermediateFb.renderTarget(0)->texture());
        inputSampler.setWrap(gl::GL_CLAMP_TO_EDGE);

        m_blurEffect.draw().setFramebuffer(m_renderManager.ssaoBuffer());
    }

    void render() override
    {
        m_projectionUniform.set(m_renderManager.mainCamera().projection());

        m_effect.schedule();
        m_blurEffect.schedule();
    }

private:
    ScreenSpaceEffect   m_effect;
    ScreenSpaceEffect   m_blurEffect;
    Framebuffer         m_intermediateFb;
    LayoutedBlob        m_samples;
    Uniform             m_projectionUniform;
};

SsaoRenderer::SsaoRenderer(RenderManager & renderManager):
    Renderer(renderManager)
{}

void SsaoRenderer::registerRenderNodes()
{
    m_renderManager.registerRenderNode(std::make_shared<SsaoNode>(*this), RenderPhase::PostGBuffer);
}

}