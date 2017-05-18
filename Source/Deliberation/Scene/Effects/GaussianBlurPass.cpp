#include <Deliberation/Scene/Effects/GaussianBlurPass.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

GaussianBlurPass::GaussianBlurPass(
        GaussianBlur & gaussianBlur,
        Framebuffer & lFb,
        Framebuffer & rFb):
    m_gaussianBlur(gaussianBlur),
    m_lFb(lFb),
    m_lSurface(lFb.colorTargets()[0].surface),
    m_rFb(lFb),
    m_rSurface(rFb.colorTargets()[0].surface)
{
    auto & drawContext = gaussianBlur.drawContext();

    m_configBuffer = drawContext.createBuffer(m_gaussianBlur.configBlockLayout());

    setStandardDeviation(1.0f);
}

u32 GaussianBlurPass::radius() const
{
    return m_radius;
}

void GaussianBlurPass::setStandardDeviation(float s)
{

}

void GaussianBlurPass::render()
{

}

}
