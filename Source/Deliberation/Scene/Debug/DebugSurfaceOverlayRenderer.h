#pragma once

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>
#include <Deliberation/Scene/ScreenSpaceEffect.h>

namespace deliberation
{

class DebugSurfaceOverlayRenderer:
    public SingleNodeRenderer
{
public:
    DebugSurfaceOverlayRenderer(RenderManager & renderManager);

    void showSurface(const Surface & surface);
    void disable();

    void render() override;

private:
    bool                m_active = false;
    ScreenSpaceEffect   m_effect;
    Sampler             m_inputSampler;
};

}