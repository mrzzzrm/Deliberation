#include <Deliberation/Scene/AmbientOcclusion/AmbientOcclusionMix.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

AmbientOcclusionMix::AmbientOcclusionMix():
    m_initialized(false)
{

}

AmbientOcclusionMix::AmbientOcclusionMix(const Surface & occlusion,
                                         const Surface & color):
    m_initialized(true)
{
    Assert(occlusion.width() == color.width() && occlusion.height() == color.height(), "");

    auto & drawContext = occlusion.drawContext();

    m_output = drawContext.createTexture2D(occlusion.width(),
                                       occlusion.height(),
                                       color.format());

    m_effect = ScreenSpaceEffect(drawContext,
                                    {deliberation::DeliberationDataPath("Data/Shaders/AmbientOcclusionMix.vert"),
                                     deliberation::DeliberationDataPath("Data/Shaders/AmbientOcclusionMix.frag")},
                                    "AmbientOcclusionMix");

    m_effect.draw().setRenderTarget(0, &m_output.surface());
    m_effect.draw().sampler("Occlusion").setTexture(occlusion.texture());
    m_effect.draw().sampler("Color").setTexture(color.texture());
}

void AmbientOcclusionMix::schedule()
{
    Assert(m_initialized, "");
    m_effect.schedule();
}

Surface & AmbientOcclusionMix::output()
{
    Assert(m_initialized, "");
    return m_output.surface();
}

const Surface & AmbientOcclusionMix::output() const
{
    Assert(m_initialized, "");
    return m_output.surface();
}

}

