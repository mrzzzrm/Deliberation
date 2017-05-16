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

    m_inputSampler = m_effect.draw().sampler("Input");
    m_inputSampler.setMagFilter(TextureFilter::Nearest);
}

void DebugSurfaceOverlayRenderer::showSurface(const Surface & surface)
{
    m_inputSampler.setTexture(surface);
    m_active = true;
}

void DebugSurfaceOverlayRenderer::disable()
{
    m_active = false;
}

void DebugSurfaceOverlayRenderer::render()
{
    if (!m_active) return;

    m_effect.render();
}

}