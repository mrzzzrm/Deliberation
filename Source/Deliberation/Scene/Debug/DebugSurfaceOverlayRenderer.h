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
    ScreenSpaceEffect   m_effect;
    Surface             m_surface;
};

}