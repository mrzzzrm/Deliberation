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

    std::vector<Framebuffer> framebuffers() override { return {m_extractFb}; }

protected:
    void onSetupRender() override;

private:
    ScreenSpaceEffect m_extractEffect;
    Framebuffer       m_extractFb;
    Surface           m_extractSurface;

    ScreenSpaceEffect           m_downscaleEffect;
    Sampler                     m_downscaleInput;
    std::vector<Framebuffer>    m_downscaleAndVBlurFbs; // First fb only used for VBlur
    std::vector<Framebuffer>    m_hblurFbs;

    GaussianBlur      m_blur;
    ScreenSpaceEffect m_applyEffect;
};

}