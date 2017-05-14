#include <Deliberation/Scene/Effects/BloomRenderer.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>

namespace deliberation
{

BloomRenderer::BloomRenderer(RenderManager & renderManager):
    SingleNodeRenderer(renderManager, RenderPhase::PreHdr, "Bloom")
{

}

void BloomRenderer::render()
{
    m_extractEffect.render();

    m_downscaleInput.setTexture(m_extractSurface);

    // Downscaling
    for (size_t b = 1; b < m_downscaleAndVBlurFbs.size(); b++)
    {
        auto & fb = m_downscaleAndVBlurFbs[b];

        m_downscaleEffect.draw().setFramebuffer(fb);
        m_downscaleEffect.render();
        m_downscaleInput.setTexture(fb.colorTargets()[0].surface);
    }

    m_blur.setInput(m_extractSurface)

    m_applyEffect.render();
}

void BloomRenderer::onSetupRender() {
    // Extract Fb
    m_extractFb = drawContext().createFramebuffer({m_renderManager.hdrBuffer().width(),
                                                       m_renderManager.hdrBuffer().height(),
                                                       {{PixelFormat_RGB_32_F, "Extracted"}}});

    m_extractSurface = m_extractFb.colorTargetRef("Extracted");

    // Extract effect
    m_extractEffect = ScreenSpaceEffect(drawContext(), {
        DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
        DeliberationDataPath("Data/Shaders/BloomExtract.frag")
    }, "BlurExtract");

    auto &extractDraw = m_extractEffect.draw();

    extractDraw.sampler("Input").setTexture(m_renderManager.hdrBuffer().colorTargetRef("Hdr"));
    extractDraw.uniform("Threshold").set(glm::vec3(1.5f, 1.5f, 1.5f));
    extractDraw.setFramebuffer(m_extractFb);

    // DownscaleEffect
    m_downscaleEffect = ScreenSpaceEffect(
        drawContext(),
        DeliberationShaderPaths("UV_Position2.vert", "Texture2dRgb.frag"),
        "BilinearDownscale"
    );
    auto & downscaleDraw = m_downscaleEffect.draw();
    downscaleDraw.sampler("Input").setMagFilter(TextureFilter::Linear);

    // DownscaleFbs
    {
        u32 width = drawContext().backbuffer().width();
        u32 height = drawContext().backbuffer().height();

        for (size_t l = 0; l < 4; l++) {

            FramebufferDesc desc;
            desc.width = width;
            desc.height = height;
            desc.colorTargetDescs = {{PixelFormat_RGB_32_F, "Color"}};

            m_downscaleAndVBlurFbs.emplace_back(drawContext().createFramebuffer(desc));

            width /= 2;
            height /= 2;
        }
    }

    m_blur = GaussianBlur(drawContext(), m_extractFb.colorTargetRef("Extracted"));

    m_applyEffect = ScreenSpaceEffect(drawContext(),
                                      {DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
                                      DeliberationDataPath("Data/Shaders/Texture2dRgb.frag")},
                                      "BloomApply");
    m_applyEffect.draw().sampler("Input").setTexture(m_blur.output().colorTargetRef("Blurred"));
    m_applyEffect.draw().state().setBlendState({BlendEquation::Add, BlendFactor::One, BlendFactor::One});
    m_applyEffect.draw().setFramebuffer(renderManager().hdrBuffer(), {{"Color", "Hdr"}});
}

}