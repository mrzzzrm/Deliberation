#include <Deliberation/Scene/Effects/GaussianBlur.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/MathUtils.h>

#include <Deliberation/Draw/DrawContext.h>

namespace deliberation
{

GaussianBlur::GaussianBlur(DrawContext & drawContext)
{
    auto program = drawContext.createProgram({
                                                 DeliberationDataPath("Data/Shaders/UV_Position2.vert"),
                                                 DeliberationDataPath("Data/Shaders/GaussianBlur.frag")
                                             });

    m_maxNumSamples = program.interface().uniformRef("Weights").arraySize();

    m_blur = ScreenSpaceEffect(drawContext, program, "GaussianBlur");
    m_inputSampler =  m_blur.draw().sampler("Input");
    m_inputSampler.setWrap(TextureWrap::ClampToEdge);
    m_inputSampler.setMagFilter(TextureFilter::Linear);
    m_inputSampler.setMinFilter(TextureFilter::Linear);
    m_horizontalUniform = m_blur.draw().uniform("Horizontal");
    m_weightsUniform = m_blur.draw().uniform("Weights");
    m_numSamplesUniform = m_blur.draw().uniform("NumSamples");
    m_offsetsUniform = m_blur.draw().uniform("Offsets");

    setStandardDeviation(1.0f);

    m_configBlockLayout = m_blur.draw().program().interface().uniformBlockRef("Config").layout();
}

void GaussianBlur::setInput(const Surface & surface)
{
    m_inputSampler.setTexture(surface);
}

void GaussianBlur::setFramebuffer(Framebuffer & framebuffer, const FramebufferBinding & fbBinding)
{
    m_blur.draw().setFramebuffer(framebuffer, fbBinding);
}

void GaussianBlur::setStandardDeviation(float s)
{
    const auto THRESHOLD = 0.005f;
    const auto var2 = 2.0f * s * s;

    const auto a = sqrt(var2 * glm::pi<float>());

    const auto b = log(a * THRESHOLD);
    Assert(b < 0.0f, "Illegal Standard Deviation");

    const auto c = -var2 * b;
    const auto d = sqrt(c);

    const auto radius = (i32)ceil(d);

    Assert(radius > 0, "Radius can't be zero");

    auto numInterpolatedSamples = 1 + ((radius - 1) % 2 == 0 ? (radius - 1) / 2 : (radius - 1) / 2 + 1);

    Assert(numInterpolatedSamples < m_maxNumSamples, "Invalid number of samples: " + std::to_string(numInterpolatedSamples));

    auto dist = GaussianSamples(s, radius);

    std::vector<float> offsets(numInterpolatedSamples);
    std::vector<float> weights(numInterpolatedSamples);

    offsets[0] = 0.0f;
    weights[0] = dist[0];

    for (size_t s = 1; s < numInterpolatedSamples; s++)
    {
        const auto baseOffset = (s - 1) * 2 + 1;

        if (baseOffset >= dist.size())
        {
            offsets[s] = baseOffset;
            weights[s] = dist[offsets[s]];
        }
        else
        {
            const auto sum = dist[baseOffset] + dist[baseOffset + 1];
            offsets[s] = baseOffset + dist[baseOffset + 1] / sum;
            weights[s] = sum / 1.0f;
        }
    }

    m_offsetsUniform.set(offsets);
    m_weightsUniform.set(weights);
    m_numSamplesUniform.set((i32)numInterpolatedSamples);
}

void GaussianBlur::renderHBlur()
{
    m_horizontalUniform.set(true);
    m_blur.render();
}

void GaussianBlur::renderVBlur()
{
    m_horizontalUniform.set(false);
    m_blur.render();
}

}