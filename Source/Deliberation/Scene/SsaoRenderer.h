#pragma once

#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{
class SsaoRenderer : public SingleNodeRenderer
{
public:
    SsaoRenderer(RenderManager & renderManager);

    void render() override;

protected:
    void onRenderImGui() override;

private:
    void init();

private:
    ScreenSpaceEffect m_effect;
    ScreenSpaceEffect m_blurEffect;
    Framebuffer       m_intermediateFb;
    LayoutedBlob      m_samples;
    Uniform           m_projectionUniform;
    bool              m_dirty = true;

    float m_sampleRadius = 0.3f;
    int   m_numSamples = 5;
    int   m_maxNumSamples = 5;
};
}