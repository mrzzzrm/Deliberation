#include <Deliberation/Scene/AmbientOcclusion/AmbientOcclusionBlur.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>

namespace deliberation
{

AmbientOcclusionBlur::AmbientOcclusionBlur():
    m_initialized(false)
{

}

AmbientOcclusionBlur::AmbientOcclusionBlur(const Surface & occlusion, const Surface & normalDepth):
    m_initialized(true),
    m_occlusion(occlusion)
{
    auto & context = m_occlusion.context();

    m_output = context.createTexture2D(m_occlusion.width(),
                                       m_occlusion.height(),
                                       PixelFormat_R_8_UN);

    m_effect = PostprocessingEffect(context,
                                    {deliberation::DeliberationDataPath("Data/Shaders/AmbientOcclusionBlur.vert"),
                                     deliberation::DeliberationDataPath("Data/Shaders/AmbientOcclusionBlur.frag")},
                                    "AmbientOcclusionBlur");

    m_effect.draw().setRenderTarget(0, &m_output.surface());
    m_effect.draw().sampler("Occlusion").setTexture(m_occlusion.texture());
    m_effect.draw().sampler("Occlusion").setWrap(gl::GL_CLAMP_TO_EDGE);

    m_effect.draw().sampler("NormalDepth").setTexture(normalDepth.texture());
    m_effect.draw().sampler("NormalDepth").setWrap(gl::GL_CLAMP_TO_EDGE);

    glm::vec2 texelSize = glm::vec2(1.0f) / glm::vec2(m_occlusion.width(), m_occlusion.height());
    m_effect.draw().uniform("TexelSize").set(texelSize);

    m_effect.draw().uniform("KernelSize").set(5);
}

void AmbientOcclusionBlur::schedule()
{
    Assert(m_initialized, "");
    m_effect.schedule();
}

Surface & AmbientOcclusionBlur::output()
{
    Assert(m_initialized, "");
    return m_output.surface();
}

const Surface & AmbientOcclusionBlur::output() const
{
    Assert(m_initialized, "");
    return m_output.surface();
}

}

