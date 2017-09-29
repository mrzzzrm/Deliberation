#pragma once

#include <Deliberation/Draw/Sampler.h>

#include <Deliberation/Scene/Effects/GaussianBlur.h>
#include <Deliberation/Scene/Effects/GaussianBlurPass.h>
#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{
class BloomRenderer : public SingleNodeRenderer
{
public:
    BloomRenderer();

    void render() override;

    void onRenderImGui() override;

protected:
    void onSetupRender() override;

private:
    ScreenSpaceEffect m_extractEffect;

    ScreenSpaceEffect m_downscaleEffect;
    Sampler           m_downscaleInput;
    std::vector<Framebuffer>
                             m_downscaleAndVBlurFbs; // First fb only used for VBlur
    std::vector<Framebuffer> m_hblurFbs;

    std::vector<i32>   m_numBlursPerLevel;
    std::vector<float> m_stdPerLevel;

    GaussianBlur                  m_blur;
    std::vector<GaussianBlurPass> m_blurPasses;
    ScreenSpaceEffect             m_applyEffect;
};
}