#include <Deliberation/Scene/Effects/BloomRenderer.h>

#include <imgui.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>

namespace deliberation
{
BloomRenderer::BloomRenderer()
    : SingleNodeRenderer(RenderPhase::PreHdr, "Bloom")
    , m_blur()
{
}

void BloomRenderer::render()
{
    m_extractEffect.render();

    m_downscaleInput.setTexture(
        m_downscaleAndVBlurFbs.front().colorTargets()[0].surface);

    // Downscaling
    for (size_t l = 1; l < m_downscaleAndVBlurFbs.size(); l++)
    {
        for (size_t b = 0; b < m_numBlursPerLevel[l]; b++)
        {
            m_blurPasses[l].render();
        }

        auto & fb = m_downscaleAndVBlurFbs[l];

        m_downscaleEffect.draw().setFramebuffer(fb);
        m_downscaleEffect.render();
        m_downscaleInput.setTexture(fb.colorTargets()[0].surface);
    }

     // Blurring
    for (size_t l = 0; l < m_downscaleAndVBlurFbs.size(); l++)
    {
        for (size_t b = 0; b < m_numBlursPerLevel[l]; b++)
        {
            m_blurPasses[l].render();
        }
    }

    m_applyEffect.render();
}

void BloomRenderer::onRenderImGui()
{
    ImGui::Columns(4, "Levels");
    ImGui::Separator();

    ImGui::Text("%s", "Level");
    ImGui::NextColumn();
    ImGui::Text("%s", "Number of Blurs");
    ImGui::NextColumn();
    ImGui::Text("%s", "Sample Standard Deviation");
    ImGui::NextColumn();
    ImGui::Text("%s", "Sample Radius");
    ImGui::NextColumn();

    ImGui::Separator();

    for (size_t l = 0; l < m_numBlursPerLevel.size(); l++)
    {
        ImGui::Text("%s", std::to_string(l).c_str());
        ImGui::NextColumn();

        ImGui::SliderInt(
            ("##BlurCount" + std::to_string(l)).c_str(),
            &m_numBlursPerLevel[l],
            0,
            10);
        ImGui::NextColumn();

        if (ImGui::SliderFloat(
                ("##StandardDeviation" + std::to_string(l)).c_str(),
                &m_stdPerLevel[l],
                0.1f,
                9.0f,
                "%.2f"))
        {
            m_blurPasses[l].setStandardDeviation(m_stdPerLevel[l]);
        }
        ImGui::NextColumn();

        ImGui::Text("%i", m_blurPasses[l].radius());
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}

void BloomRenderer::onSetupRender()
{
    // DownscaleEffect
    m_downscaleEffect = ScreenSpaceEffect(
        DeliberationShaderPaths({"UV_Position2.vert", "Texture2dRgb.frag"}),
        "BilinearDownscale");
    auto & downscaleDraw = m_downscaleEffect.draw();
    m_downscaleInput = downscaleDraw.sampler("Input");
    m_downscaleInput.setMagFilter(TextureFilter::Linear);
    m_downscaleInput.setMinFilter(TextureFilter::Linear);
    m_downscaleInput.setWrap(TextureWrap::ClampToEdge);

    // Framebuffers
    size_t numLevels = 4;
    {
        u32 width = GetGlobal<DrawContext>()->backbuffer().width();
        u32 height = GetGlobal<DrawContext>()->backbuffer().height();

        m_numBlursPerLevel.resize(numLevels, 1);
        m_stdPerLevel.resize(numLevels, 1.0f);
        m_blurPasses.reserve(numLevels);

        for (size_t l = 0; l < numLevels; l++)
        {
            FramebufferDesc desc;
            desc.width = width;
            desc.height = height;
            desc.colorTargetDescs = {{PixelFormat_RGB_32_F, "Color"}};

            desc.name = "ScaleVBlur" + std::to_string(l);
            m_downscaleAndVBlurFbs.emplace_back(
                GetGlobal<DrawContext>()->createFramebuffer(desc));

            desc.name = "HBlur" + std::to_string(l);
            m_hblurFbs.emplace_back(GetGlobal<DrawContext>()->createFramebuffer(desc));

            GetGlobal<RenderManager>()->registerFramebuffer(m_downscaleAndVBlurFbs.back());
            GetGlobal<RenderManager>()->registerFramebuffer(m_hblurFbs.back());

            m_blurPasses.emplace_back(
                m_blur, m_downscaleAndVBlurFbs.back(), m_hblurFbs.back());

            width /= 2;
            height /= 2;
        }
    }

    // Extract effect
    m_extractEffect = ScreenSpaceEffect(
        DeliberationShaderPaths({"UV_Position2.vert", "BloomExtract.frag"}),
        "BloomExtract");

    auto & extractDraw = m_extractEffect.draw();

    extractDraw.sampler("Input").setTexture(
        GetGlobal<RenderManager>()->hdrBuffer().colorTargetRef("Hdr"));
    extractDraw.uniform("Threshold").set(1.5f);
    extractDraw.setFramebuffer(
        m_downscaleAndVBlurFbs[0], {{"Extracted", "Color"}});

    // Apply
    m_applyEffect = ScreenSpaceEffect(
        DeliberationShaderPaths({"UV_Position2.vert", "BloomApply.frag"}),
        "BloomApply");
    m_applyEffect.draw().sampler("InputA").setTexture(
        m_downscaleAndVBlurFbs[0].colorTargets()[0].surface);
    m_applyEffect.draw().sampler("InputB").setTexture(
        m_downscaleAndVBlurFbs[1].colorTargets()[0].surface);
    m_applyEffect.draw().sampler("InputC").setTexture(
        m_downscaleAndVBlurFbs[2].colorTargets()[0].surface);
    m_applyEffect.draw().sampler("InputD").setTexture(
        m_downscaleAndVBlurFbs[3].colorTargets()[0].surface);
    m_applyEffect.draw().state().setBlendState(
        {BlendEquation::Add, BlendFactor::One, BlendFactor::One});
    m_applyEffect.draw().setFramebuffer(
        GetGlobal<RenderManager>()->hdrBuffer(), {{"Color", "Hdr"}});
}
}