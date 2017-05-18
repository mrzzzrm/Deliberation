#pragma once

#include <Deliberation/Draw/Sampler.h>

#include <Deliberation/Scene/Effects/GaussianBlur.h>
#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class BloomRenderer : public SingleNodeRenderer
{
public:
    BloomRenderer(RenderManager & renderManager);

    void render() override;

    void renderDebugGui() override;

protected:
    void onSetupRender() override;
    void blurLevel(size_t level);

private:
    ScreenSpaceEffect m_extractEffect;
    Framebuffer       m_extractFb;
    Surface           m_extractSurface;

    ScreenSpaceEffect           m_downscaleEffect;
    Sampler                     m_downscaleInput;
    std::vector<Framebuffer>    m_downscaleAndVBlurFbs; // First fb only used for VBlur
    std::vector<Framebuffer>    m_hblurFbs;

    std::vector<i32>            m_numBlursPerLevel;
    std::vector<float>          m_stdPerLevel;

    GaussianBlur      m_blur;
    ScreenSpaceEffect m_applyEffect;
};

}