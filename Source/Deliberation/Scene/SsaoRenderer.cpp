#include <Deliberation/Scene/SsaoRenderer.h>

#include <imgui.h>

#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/Core/Math/MathUtils.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/TextureLoader.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

SsaoRenderer::SsaoRenderer(RenderManager & renderManager):
    SingleNodeRenderer(renderManager, RenderPhase::PostGBuffer, "SSAO")
{

}

void SsaoRenderer::render()
{
    if (m_dirty) init();

    m_effect.draw().uniform("Radius").set(m_sampleRadius);
    m_effect.draw().uniform("NumSamples").set(m_numSamples);
    m_projectionUniform.set(m_renderManager.mainCamera().projection());

    m_effect.schedule();
    m_blurEffect.schedule();
}

void SsaoRenderer::renderDebugGui()
{
    ImGui::SliderFloat("Sample Radius", &m_sampleRadius, 0.005f, 10.0f, "%.3f", 1.5f);
    ImGui::SliderInt("Samples", &m_numSamples, 3, m_maxNumSamples);
}

void SsaoRenderer::init()
{
    auto & drawContext = m_renderManager.drawContext();


    m_intermediateFb = drawContext.createFramebuffer({drawContext.backbuffer().width(), drawContext.backbuffer().height(),
                                                      {{PixelFormat_R_32_F, "UnblurredSsao"}}});

    m_effect = ScreenSpaceEffect(m_renderManager.drawContext(),
                                 {DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
                                  DeliberationDataPath("Data/Shaders/Ssao.frag")},
                                 "SSAO");

    auto positionSampler = m_effect.draw().sampler("Position");
    positionSampler.setTexture(m_renderManager.gbuffer().colorTargetRef("Position"));
    positionSampler.setWrap(gl::GL_CLAMP_TO_EDGE);

    auto normalSampler = m_effect.draw().sampler("Normal");
    normalSampler.setTexture(m_renderManager.gbuffer().colorTargetRef("Normal"));
    normalSampler.setWrap(gl::GL_CLAMP_TO_EDGE);

    m_effect.draw().setFramebuffer(m_intermediateFb);

    const auto & interface = m_effect.draw().program().interface();

    // Samples
    m_maxNumSamples = interface.uniform("Samples")->arraySize();
    const auto numSamples = std::min<int>(5, m_maxNumSamples);
    m_samples = LayoutedBlob({"Sample", Type_Vec3}, m_maxNumSamples);
    auto samples = m_samples.iterator<glm::vec3>("Sample");

    for (size_t s = 0; s < m_maxNumSamples; s++)
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
    m_effect.draw().uniform("Bias").set(0.02f);
    m_effect.draw().uniform("NoiseEnabled").set(true);
    m_projectionUniform = m_effect.draw().uniform("Projection");

    // Blur
    m_blurEffect = ScreenSpaceEffect(drawContext, {DeliberationDataPath("Data/Shaders/Blur4x4.frag"),
                                                   DeliberationDataPath("Data/Shaders/UV_Position2.vert")}, "SSAO Blur");

    auto inputSampler = m_blurEffect.draw().sampler("Input");
    inputSampler.setTexture(m_intermediateFb.colorTargetRef("UnblurredSsao"));
    inputSampler.setWrap(gl::GL_CLAMP_TO_EDGE);

    m_blurEffect.draw().setFramebuffer(m_renderManager.ssaoBuffer());

    m_dirty = false;
}

}