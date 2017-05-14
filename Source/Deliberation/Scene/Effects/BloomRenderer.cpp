#include <Deliberation/Scene/Effects/BloomRenderer.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>

namespace deliberation
{

BloomRenderer::BloomRenderer(RenderManager & renderManager):
    SingleNodeRenderer(renderManager, RenderPhase::PreHdr, "Bloom"),
    m_blur(renderManager.drawContext())
{

}

void BloomRenderer::render()
{
    m_extractEffect.render();

    m_downscaleInput.setTexture(m_downscaleAndVBlurFbs.front().colorTargets()[0].surface);

    // Downscaling
    for (size_t l = 1; l < m_downscaleAndVBlurFbs.size(); l++)
    {
        auto & fb = m_downscaleAndVBlurFbs[l];

        m_downscaleEffect.draw().setFramebuffer(fb);
        m_downscaleEffect.render();
        m_downscaleInput.setTexture(fb.colorTargets()[0].surface);
    }

    // Blurring
    for (size_t l = 0; l < m_downscaleAndVBlurFbs.size(); l++)
    {
        auto & lFb = m_downscaleAndVBlurFbs[l];
        auto & rFb = m_downscaleAndVBlurFbs[l];

        m_blur.setInput(lFb.colorTargets()[0].surface);
        m_blur.setFramebuffer(rFb, {{"Blurred", "Color"}}); // TODO: don't create FB-binding dynamically
        m_blur.renderHBlur();

        m_blur.setInput(rFb.colorTargets()[0].surface);
        m_blur.setFramebuffer(lFb, {{"Blurred", "Color"}});// TODO: don't create FB-binding dynamically
        m_blur.renderVBlur();
    }

    m_applyEffect.render();
}

void BloomRenderer::onSetupRender() {
    // DownscaleEffect
    m_downscaleEffect = ScreenSpaceEffect(
        drawContext(),
        DeliberationShaderPaths({"UV_Position2.vert", "Texture2dRgb.frag"}),
        "BilinearDownscale"
    );
    auto & downscaleDraw = m_downscaleEffect.draw();
    m_downscaleInput = downscaleDraw.sampler("Input");
    m_downscaleInput.setMagFilter(TextureFilter::Linear);

    // Framebuffers
    {
        u32 width = drawContext().backbuffer().width();
        u32 height = drawContext().backbuffer().height();

        for (size_t l = 0; l < 4; l++)
        {

            FramebufferDesc desc;
            desc.width = width;
            desc.height = height;
            desc.colorTargetDescs = {{PixelFormat_RGB_32_F, "Color"}};

            m_downscaleAndVBlurFbs.emplace_back(drawContext().createFramebuffer(desc));
            m_hblurFbs.emplace_back(drawContext().createFramebuffer(desc));

            width /= 2;
            height /= 2;
        }
    }

    // Extract effect
    m_extractEffect = ScreenSpaceEffect(drawContext(),
        DeliberationShaderPaths({"UV_Position2.vert", "BloomExtract.frag"}),
     "BlurExtract");

    auto &extractDraw = m_extractEffect.draw();

    extractDraw.sampler("Input").setTexture(m_renderManager.hdrBuffer().colorTargetRef("Hdr"));
    extractDraw.uniform("Threshold").set(glm::vec3(1.5f, 1.5f, 1.5f));
    extractDraw.setFramebuffer(m_downscaleAndVBlurFbs[0], {{"Extracted", "Color"}});

    // Apply
    m_applyEffect = ScreenSpaceEffect(drawContext(),
                                      DeliberationShaderPaths({"UV_Position2.vert", "BloomApply.frag"}),
                                      "BloomApply");
    m_applyEffect.draw().sampler("InputA").setTexture(m_downscaleAndVBlurFbs[0].colorTargets()[0].surface);
    m_applyEffect.draw().sampler("InputB").setTexture(m_downscaleAndVBlurFbs[1].colorTargets()[0].surface);
    m_applyEffect.draw().sampler("InputC").setTexture(m_downscaleAndVBlurFbs[2].colorTargets()[0].surface);
    m_applyEffect.draw().sampler("InputD").setTexture(m_downscaleAndVBlurFbs[3].colorTargets()[0].surface);
    m_applyEffect.draw().state().setBlendState({BlendEquation::Add, BlendFactor::One, BlendFactor::One});
    m_applyEffect.draw().setFramebuffer(renderManager().hdrBuffer(), {{"Color", "Hdr"}});
}

}