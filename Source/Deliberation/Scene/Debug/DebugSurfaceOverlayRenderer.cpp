#include <Deliberation/Scene/Debug/DebugSurfaceOverlayRenderer.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Surface.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>

namespace deliberation
{

DebugSurfaceOverlayRenderer::DebugSurfaceOverlayRenderer(RenderManager & renderManager):
    SingleNodeRenderer(renderManager, RenderPhase::PreOverlay, "SurfaceOverlay")
{
    auto & drawContext = renderManager.drawContext();

    m_effect = ScreenSpaceEffect(drawContext, {
        DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
        DeliberationDataPath("Data/Shaders/DebugOverlaySurface.frag")
    }, "DebugSurfaceOverlay");
}

void DebugSurfaceOverlayRenderer::showSurface(const Surface & surface)
{
    m_surface = surface;
}

void DebugSurfaceOverlayRenderer::disable()
{
    m_surface = Surface();
}

void DebugSurfaceOverlayRenderer::render()
{
    if (!m_surface.isInitialized()) return;

    m_effect.draw().sampler("Input").setTexture(m_surface);
    m_effect.render();
}

}