#include <Deliberation/Scene/AmbientOcclusion/AmbientOcclusion.h>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/Random.h>

#include <Deliberation/Draw/Context.h>
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
                                   const Camera3D & camera):
    m_initialized(true),
    m_normalDepth(normalDepth),
    m_color(color),
    m_camera(&camera)
{
    auto & context = m_normalDepth.context();

    m_output = context.createTexture2D(m_normalDepth.width(),
                                       m_normalDepth.height(),
                                       PixelFormat_R_8_U);
    m_effect = PostprocessingEffect(context,
                                    {deliberation::dataPath("Data/Shaders/AmbientOcclusion.vert"),
                                     deliberation::dataPath("Data/Shaders/AmbientOcclusion.frag")},
                                    "AmbientOcclusion");
    m_effect.draw().setRenderTarget(0, &m_output.surface());
    m_effect.draw().sampler("NormalDepth").setTexture(m_normalDepth.texture());
    m_projection = m_effect.draw().uniform("Projection");

    m_sampleRadius = m_effect.draw().uniform("SampleRadius");
    m_sampleRadius.set(0.25f);

    SurfaceBinary surface(4, 4, PixelFormat_RGB_8_I);
    for (auto y = 0; y < surface.height(); y++)
    {
        for (auto x = 0; x < surface.width(); x++)
        {
            glm::vec3 pixel = glm::vec3(
                RandomFloat(-1.0f, 1.0f) * 127,
                RandomFloat(-1.0f, 1.0f) * 127,
                0.0f * 127
            );

            pixel = glm::normalize(pixel);

            surface.setPixel(x, y, pixel);
        }
    }

    m_rotation = context.createTexture2D(4, 4, surface.format());
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
            RandomFloat( 0.0f, 1.0f)
        );
        sample = glm::normalize(sample);

        auto scale = (float)s / (float)kernel.size();
        scale = glm::mix(0.1f, 1.0f, scale * scale);

        sample *= scale;
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
   // return m_output.surface();
}

const Surface & AmbientOcclusion::output() const
{
    Assert(m_initialized, "");
    return m_mix.output();
   // return m_output.surface();
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

