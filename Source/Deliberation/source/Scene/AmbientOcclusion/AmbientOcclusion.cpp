#include <Deliberation/Scene/AmbientOcclusion/AmbientOcclusion.h>

#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/Core/Math/Vec.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/SurfaceBinary.h>

#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{

AmbientOcclusion::AmbientOcclusion():
    m_initialized(false)
{

}

AmbientOcclusion::AmbientOcclusion(const Surface & normalDepth,
                                   const Surface & color,
                                   const Surface & positionVS,
                                   const Camera3D & camera):
    m_initialized(true),
    m_normalDepth(normalDepth),
    m_positionVS(positionVS),
    m_color(color),
    m_camera(&camera)
{
    auto & drawContext = m_normalDepth.drawContext();

    m_output = drawContext.createTexture2D(m_normalDepth.width(),
                                       m_normalDepth.height(),
                                       PixelFormat_RGB_32_F);
    m_effect = ScreenSpaceEffect(drawContext,
                                    {deliberation::DeliberationDataPath("Data/Shaders/AmbientOcclusion.vert"),
                                     deliberation::DeliberationDataPath("Data/Shaders/AmbientOcclusion.frag")},
                                    "AmbientOcclusion");
    m_effect.draw().setRenderTarget(0, &m_output.surface());

    m_effect.draw().sampler("NormalDepth").setTexture(m_normalDepth.texture());
    m_effect.draw().sampler("NormalDepth").setWrap(gl::GL_CLAMP_TO_EDGE);

    m_effect.draw().sampler("PositionVS").setTexture(m_positionVS.texture());
    m_effect.draw().sampler("PositionVS").setWrap(gl::GL_CLAMP_TO_EDGE);

    m_projection = m_effect.draw().uniform("Projection");

    m_sampleRadius = m_effect.draw().uniform("SampleRadius");
    m_sampleRadius.set(0.1f);

    SurfaceBinary surface(4, 4, PixelFormat_RG_8_SN);
    for (auto y = 0; y < surface.height(); y++)
    {
        for (auto x = 0; x < surface.width(); x++)
        {
            glm::vec2 pixel = glm::vec2(
                RandomFloat(-1.0f, 1.0f),
                RandomFloat(-1.0f, 1.0f)
            );

            pixel = glm::normalize(pixel);

            int8_t r = std::floor(pixel.r * 128.0f);
            int8_t g = std::floor(pixel.g * 128.0f);

            surface.setPixel(x, y, ByteVec2(r, g));
        }
    }

    m_rotation = drawContext.createTexture2D(4, 4, surface.format());
    m_rotation.scheduleUpload(surface);
    m_effect.draw().sampler("Rotation").setTexture(m_rotation);

    m_rotationCoordScale = m_effect.draw().uniform("RotationCoordScale");
    m_rotationCoordScale.set(glm::vec2((float)m_normalDepth.width()/(float)m_rotation.width(),
                                       (float)m_normalDepth.height()/(float)m_rotation.height()));

    std::vector<glm::vec3> kernel(32);
    for (auto s = 0; s < kernel.size(); s++)
    {
        auto & sample = kernel[s];
        sample = glm::vec3(
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(0.0f, 1.0f)
        );
        sample = glm::normalize(sample);

        auto scale = (float)s / (float)kernel.size();
        scale = glm::mix(0.1f, 1.0f, scale * scale);

        sample *= scale;

        std::cout << "Sample " << s << ": " << sample << std::endl;
    }

    m_effect.draw().uniform("Kernel").set(kernel);

    m_blur = AmbientOcclusionBlur(m_output.surface(), m_normalDepth);
    m_mix = AmbientOcclusionMix(m_blur.output(), m_color);
}

Surface & AmbientOcclusion::rawOcclusion()
{
    Assert(m_initialized, "");
    return m_output.surface();
}

const Surface & AmbientOcclusion::rawOcclusion() const
{
    Assert(m_initialized, "");
    return m_output.surface();
}

Surface & AmbientOcclusion::blurredOcclusion()
{
    Assert(m_initialized, "");
    return m_blur.output();
}

const Surface & AmbientOcclusion::blurredOcclusion() const
{
    Assert(m_initialized, "");
    return m_blur.output();
}

Surface & AmbientOcclusion::output()
{
    Assert(m_initialized, "");
    return m_mix.output();
    //return m_output.surface();
}

const Surface & AmbientOcclusion::output() const
{
    Assert(m_initialized, "");
    return m_mix.output();
   // return m_output.surface();
}

void AmbientOcclusion::setSampleRadius(float radius)
{
    m_sampleRadius.set(radius);
}

void AmbientOcclusion::schedule()
{
    Assert(m_initialized, "");

    m_projection.set(m_camera->projection());

    m_effect.schedule();
    m_blur.schedule();
    m_mix.schedule();
}

}

